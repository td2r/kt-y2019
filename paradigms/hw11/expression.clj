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
  (toStr []))

(declare ZERO)

(deftype JConstant [value]
  IExpression
  (evaluate [this vvalues] (double value))
  (diff [this dv] ZERO)
  (toStr [this] (format "%.1f" (double value))))

(def ZERO (JConstant. 0.0))
(def ONE (JConstant. 1.0))
(def TWO (JConstant. 2.0))

(defn Constant [value] (JConstant. value))

(deftype JVariable [vname]
  IExpression
  (evaluate [this vvalues] (get vvalues vname))
  (diff [this dv] (if (= dv vname) ONE ZERO))
  (toStr [this] vname))

(def VX (JVariable. "x"))
(def VY (JVariable. "y"))
(def VZ (JVariable. "z"))

(defn Variable [vname] (JVariable. vname))

(deftype JOperation [f fsymbol fdiff terms]
  IExpression
  (evaluate [this vvalues] (apply f (mapv #(.evaluate % vvalues) terms)))
  (diff [this dv] (fdiff terms (mapv #(.diff % dv) terms)))
  (toStr [this] (str "(" (clojure.string/join " " (cons fsymbol (map #(.toStr %) terms))) ")")))

(deftype JUnaryOperation [f fsymbol fdiff term]
  IExpression
  (evaluate [this vvalues] (f (.evaluate term vvalues)))
  (diff [this dv] (fdiff term (.diff term dv)))
  (toStr [this] (str "(" fsymbol " " (.toStr term) ")")))

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
(def Multiply (JCreateOperation * "*" (fn [terms terms'] (floating-comma-diff Add terms terms'))))
(def Square (JCreateUnaryOperation #(* % %) "square" (fn [term term'] (Multiply TWO term term'))))
(def Divide (JCreateOperation
            (fn
              ([x] (/ (double x)))
              ([x & d] (/ (double x) (double (apply * d)))))
            "/"
            (fn [terms terms']
              (Divide (floating-comma-diff Subtract terms terms') (Square (apply Multiply (rest terms)))))))
(def Negate (JCreateUnaryOperation #(- %) "negate" (fn [_ term'] (Negate term'))))
(def Exp (JCreateUnaryOperation #(Math/exp %) "exp" (fn [term term'] (Multiply term' (Exp term)))))
(def Sumexp (JCreateOperation +exp "sumexp"
            (fn [terms terms'] (apply Add (mapv Multiply terms' (mapv Exp terms))))))
(def Softmax (JCreateOperation softargmax "softmax"
             (fn [[z & terms] [z' & terms']]
               (Divide
                 (Multiply (Exp z) (apply Add (mapv #(Multiply (Subtract z' %1) (Exp %2)) terms' terms)))
                 (Square (apply Sumexp z terms))))))


(defn evaluate [expr variables] (.evaluate expr variables))
(defn diff [expr dv] (.diff expr dv))
(defn toString [expr] (.toStr expr))

(def OBJECT_OPERATION
  {'+ Add
   '- Subtract
   '* Multiply
   '/ Divide
   'negate Negate
   'sumexp Sumexp
   'softmax Softmax})

(def OBJECT_VARIABLES
  {'x VX
   'y VY
   'z VZ})

(defn prefixParse [operations variables const]
  (letfn [(parseImpl [arg]
            (cond
              (number? arg) (const arg)
              (contains? variables arg) (get variables arg)
              :else (apply (get operations (first arg)) (mapv parseImpl (rest arg)))))]
    (fn [str] (parseImpl (read-string str)))))
(def parseFunction (prefixParse FUNCTIONAL_OPERATION FUNCTIONAL_VARIABLES constant))
(def parseObject (prefixParse OBJECT_OPERATION OBJECT_VARIABLES Constant))