(defn -return [value tail] {:value value :tail tail})
(def -valid? boolean)
(def -value :value)
(def -tail :tail)

(defn _show [result]
 (if (-valid? result) (str "-> " (pr-str (-value result)) " | " (pr-str (apply str (-tail result))))
                      "!"))
(defn tabulate [parser inputs]
 (run! (fn [input] (printf "    %-10s %s\n" (pr-str input) (_show (parser input)))) inputs))
(defn _empty [value] (partial -return value))
(defn _char [p]
 (fn [[c & cs]]
   (if (and c (p c)) (-return c cs))))
(defn _map [f result]
 (if (-valid? result)
   (-return (f (-value result)) (-tail result))))
(defn _combine [f a b]
 (fn [str]
   (let [ar ((force a) str)]
     (if (-valid? ar)
       (_map (partial f (-value ar))
             ((force b) (-tail ar)))))))
(defn _either [a b]
 (fn [str]
   (let [ar ((force a) str)]
     (if (-valid? ar) ar ((force b) str)))))
(defn _parser [p]
 (fn [input]
   (-value ((_combine (fn [v _] v) p (_char #{\u0000})) (str input \u0000)))))

(defn +char [chars] (_char (set chars)))
(defn +char-not [chars] (_char (comp not (set chars))))
(defn +map [f parser] (comp (partial _map f) parser))
(def +parser _parser)
(def +ignore (partial +map (constantly 'ignore)))
(defn iconj [coll value]
 (if (= value 'ignore) coll (conj coll value)))
(defn +seq [& ps]
 (reduce (partial _combine iconj) (_empty []) ps))
(defn +seqf [f & ps] (+map (partial apply f) (apply +seq ps)))
(defn +seqn [n & ps] (apply +seqf (fn [& vs] (nth vs n)) ps))
(defn +or [p & ps]
 (reduce _either p ps))
(defn +opt [p]
 (+or p (_empty nil)))
(defn +star [p]
 (letfn [(rec [] (+or (+seqf cons p (delay (rec))) (_empty ())))] (rec)))
(defn +plus [p] (+seqf cons p (+star p)))
(defn +str [p] (+map (partial apply str) p))





(defn constant [value] (fn [vals] value))
(defn variable [vn] (fn [vals] (get vals vn)))
(def vx (variable "x"))
(def vy (variable "y"))
(def vz (variable "z"))

(defn operation [agg] (fn [& terms] (fn [vals] (apply agg (mapv (fn [f] (f vals)) terms)))))

(def add (operation +))
(def subtract (operation -))
(def multiply (operation *))
(def divide (operation (fn
                         ([x] (/ (double x)))
                         ([x & d] (/ (double x) (double (apply * d)))))))
(def negate (operation #(- %)))

(defn +exp [& args] (apply + (mapv #(Math/exp %) args)))
(defn softargmax [z & terms] (/ (double (Math/exp z)) (double (apply +exp z terms))))
(def sumexp (operation +exp))
(def softmax (operation softargmax))

(def FUNCTIONAL_OPERATION
  {'+ add
   '- subtract
   '* multiply
   '/ divide
   'negate negate
   'sumexp sumexp
   'softmax softmax})

(def FUNCTIONAL_VARIABLES
  {'x vx
   'y vy
   'z vz})

(definterface IExpression
  (evaluate [vvalues])
  (diff [dv])
  (toStr [])
  (toStrInfix []))

(declare ZERO)

(deftype JConstant [value]
  IExpression
  (evaluate [this vvalues] (double value))
  (diff [this dv] ZERO)
  (toStr [this] (format "%.1f" (double value)))
  (toStrInfix [this] (format "%.1f" (double value))))

(def ZERO (JConstant. 0.0))
(def ONE (JConstant. 1.0))
(def TWO (JConstant. 2.0))

(defn Constant [value] (JConstant. value))

(deftype JVariable [vname]
  IExpression
  (evaluate [this vvalues] (get vvalues vname))
  (diff [this dv] (if (= dv vname) ONE ZERO))
  (toStr [this] vname)
  (toStrInfix [this] vname))

(def VX (JVariable. "x"))
(def VY (JVariable. "y"))
(def VZ (JVariable. "z"))

(defn Variable [vname] (JVariable. vname))

(deftype JOperation [f fsymbol fdiff terms]
  IExpression
  (evaluate [this vvalues] (apply f (mapv #(.evaluate % vvalues) terms)))
  (diff [this dv] (fdiff terms (mapv #(.diff % dv) terms)))
  (toStr [this] (str "(" (clojure.string/join " " (cons fsymbol (map #(.toStr %) terms))) ")"))
  (toStrInfix [this] (str "(" (clojure.string/join (str " " fsymbol " ") (map #(.toStrInfix %) terms)) ")")))

(deftype JUnaryOperation [f fsymbol fdiff term]
  IExpression
  (evaluate [this vvalues] (f (.evaluate term vvalues)))
  (diff [this dv] (fdiff term (.diff term dv)))
  (toStr [this] (str "(" fsymbol " " (.toStr term) ")"))
  (toStrInfix [this] (str fsymbol "(" (.toStrInfix term) ")")))

(defn JCreateOperation [f fsymbol fdiff] (fn [& terms] (JOperation. f fsymbol fdiff terms)))
(defn JCreateUnaryOperation [f fsymbol fdiff] (fn [term] (JUnaryOperation. f fsymbol fdiff term)))

(def Add (JCreateOperation + "+" (fn [_ terms'] (apply Add terms'))))
(def Subtract (JCreateOperation - "-" (fn [_ terms'] (apply Subtract terms'))))
(declare Multiply)
(defn floating-comma-diff [agg terms terms']
  (first
    (reduce
      (fn [[x'y?xy' xy] [z' z]] [(agg (Multiply x'y?xy' z) (Multiply xy z')) (Multiply xy z)])
      (mapv vector terms' terms))))
(def Multiply (JCreateOperation * "*"
                                (partial floating-comma-diff Add)))
(def Square (JCreateUnaryOperation #(* % %) "square"
                                   (fn [term term'] (Multiply TWO term term'))))
(def Negate (JCreateUnaryOperation #(- %) "negate"
                                   (fn [_ term'] (Negate term'))))
(def Divide (JCreateOperation
              (fn
                ([x] (/ (double x)))
                ([x & d] (/ (double x) (double (apply * d)))))
              "/"
              (fn [terms terms'] (if (= 1 (count terms))
                                   (Divide (apply Negate terms') (apply Square terms))
                                   (Divide
                                     (floating-comma-diff Subtract terms terms')
                                     (Square (apply Multiply (rest terms))))))))
(def Exp (JCreateUnaryOperation #(Math/exp %) "exp"
                                (fn [term term'] (Multiply term' (Exp term)))))
(def Sumexp (JCreateOperation +exp "sumexp"
                              (fn [terms terms'] (apply Add (mapv Multiply terms' (mapv Exp terms))))))
(def Softmax (JCreateOperation softargmax "softmax"
                               (fn [[z & terms] [z' & terms']]
                                 (Divide
                                   (Multiply (Exp z) (apply Add (mapv #(Multiply (Subtract z' %1) (Exp %2)) terms' terms)))
                                   (Square (apply Sumexp z terms))))))

(defn double-bitwise [f]
  (fn [& args] (Double/longBitsToDouble (apply f (mapv #(Double/doubleToLongBits %) args)))))
(def double-and (double-bitwise bit-and))
(def double-or (double-bitwise bit-or))
(def double-xor (double-bitwise bit-xor))
(def impl (double-bitwise #(bit-or (bit-not %1) %2)))
(def iff (double-bitwise (comp bit-not bit-xor)))
(def And (JCreateOperation double-and "&" nil))
(def Or (JCreateOperation double-or "|" nil))
(def Xor (JCreateOperation double-xor "^" nil))
(def Impl (JCreateOperation impl "=>" nil))
(def Iff (JCreateOperation iff "<=>" nil))

(defn evaluate [expr variables] (.evaluate expr variables))
(defn diff [expr dv] (.diff expr dv))
(defn toString [expr] (.toStr expr))
(defn toStringInfix [expr] (.toStrInfix expr))

(def OBJECT_OPERATION
  {'+           Add
   '-           Subtract
   '*           Multiply
   '/           Divide
   'negate      Negate
   'sumexp      Sumexp
   'softmax     Softmax
   '&           And
   '|           Or
   (symbol "^") Xor
   '=>          Impl
   '<=>         Iff})

(def OBJECT_VARIABLES
  {'x VX
   'y VY
   'z VZ})

(defn prefixParse [operations variables const]
  (letfn [(parseImpl [arg]
            (cond
              (number? arg) (const arg)
              (contains? variables arg) (get variables arg)
              (seq? arg) (apply (get operations (first arg)) (mapv parseImpl (rest arg)))))]
    (fn [str] (parseImpl (read-string str)))))
(def parseFunction (prefixParse FUNCTIONAL_OPERATION FUNCTIONAL_VARIABLES constant))
(def parseObject (prefixParse OBJECT_OPERATION OBJECT_VARIABLES Constant))

(def *digit (+char "0123456789"))
(def *space (+char " \t\n\r"))
(def *letter (+char (apply str (filter #(Character/isLetter %) (mapv char (range 32 128))))))

(def *ws (+ignore (+star *space)))
(def *number (+map read-string (+str
             (+seqf
               concat
               (+opt (+seq (+char "-")))
               (+plus *digit)
               (+opt (+seqf cons (+char ".") (+plus *digit)))))))
(def *constant (+map Constant *number))
(def *variable (+map
                 #(OBJECT_VARIABLES ((comp symbol str) %))
                 (+char "xyz")))
(defn *operation [s] (+map
                       #(OBJECT_OPERATION (apply (comp symbol str) %))
                       (apply +seq (mapv (comp +char str) (seq s)))))

(declare *iff)
(def *primitive
  (+seqn 0 (+opt *ws)
         (+or
           *constant
           *variable
           (+seqn 1 (+char "(") (+opt *ws) (delay *iff) (+opt *ws) (+char ")"))
           (+seqf #(%1 %2) (*operation "negate") (+opt *ws) (delay *primitive))) (+opt *ws)))

(defn *llayer [*uplayer & op_names]
  (let [*ops (map *operation op_names)]
    (+seqf
      (partial reduce #((first %2) %1 (last %2)))
      *uplayer (+star (+seqf vector *ws (delay (apply +or *ops)) *ws *uplayer)))))

(defn *rlayer [*uplayer & op_names]
  (let [*ops (map *operation op_names)]
    (+seqf
      (fn [h t] (if (nil? t) h ((first t) h (second t))))
      *uplayer
      (+opt (+seq *ws (apply +or *ops) *ws (delay (apply *rlayer *uplayer op_names)))))))

(def layers [[*llayer "*" "/"]
              [*llayer "+" "-"]
              [*llayer "&"]
              [*llayer "|"]
              [*llayer "^"]
              [*rlayer "=>"]
              [*llayer "<=>"]])

(def *iff (reduce (fn [res [layer_type & strs]] (apply layer_type res strs)) *primitive layers))
(def parseObjectInfix (+parser *iff))
