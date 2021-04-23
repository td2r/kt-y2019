(defn constant [value] (fn [vals] value))
(defn variable [vn] (fn [vals] (get vals vn)))
(def vx (variable "x"))
(def vy (variable "y"))
(def vz (variable "z"))

(defn operation [agg] (fn [& terms] (fn [vals] (apply agg (mapv (fn [f] (f vals)) terms)))))

(def add (operation +))
(def subtract (operation -))
(def multiply (operation *))
; :NOTE: Только бинарный? :FIXED
(def divide (operation (fn
                         ([x] (/ (double x)))
                         ([x & d] (/ (double x) (double (apply * d)))))))
(def negate (operation #(- %)))

(defn exp+ [& args] (apply + (mapv #(Math/exp %) args)))
(def sumexp (operation exp+))
(def softmax (operation (fn [& args] (/ (Math/exp (first args)) (apply exp+ args)))))

(def OPERATION
  {'+ add
   '- subtract
   '* multiply
   '/ divide
   'negate negate
   'sumexp sumexp
   'softmax softmax})

(def VARIABLES
  {'x vx
  'y vy
  'z vz})

(defn parseImpl [arg]
  (cond
    (number? arg) (constant arg)
    (contains? VARIABLES arg) (get VARIABLES arg)
    :else (apply (get OPERATION (first arg)) (mapv parseImpl (rest arg)))))
(defn parseFunction [str] (parseImpl (read-string str)))