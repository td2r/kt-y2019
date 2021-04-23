(defn check-dimensions [vectors] (apply == (mapv count vectors)))

(defn coordinate-wise-apply [f obj-type?]
  (fn [& elements]
    {:pre [(every? obj-type? elements) (check-dimensions elements)]
     :post [(vector? %)]}
    (apply mapv f elements)))

(defn vv-apply [f] (coordinate-wise-apply f vector?))
(defn matrix? [x] (and (vector? x) (every? vector? x) (apply == (mapv count x))))
(defn mm-apply [f] (coordinate-wise-apply (vv-apply f) matrix?))

(def v+ "Sum of vectors." (vv-apply +))
(def v- "Vector difference." (vv-apply -))
(def v* "Coordinate-wise product of vectors." (vv-apply *))

(defn scalar "Scalar product of vectors." [& vectors] (apply + (apply v* vectors)))
(defn vect "Vector product." [& vectors] {:pre [(every? #(and (vector? %) (== 3 (count %))) vectors)]}
  (reduce (fn [[x1 y1 z1] [x2 y2 z2]] (vector (- (* y1 z2) (* y2 z1))  (- (* x2 z1) (* x1 z2)) (- (* x1 y2) (* x2 y1)))) vectors))
(defn v*s "The product of a vector by a scalars." [v & scalars] (mapv (partial * (apply * scalars)) v))

; :NOTE: Матрица [[1] [1 2]] :SOLVED
(def m+ "Sum of matrices." (mm-apply +))
(def m- "Matrix difference." (mm-apply -))
(def m* "Coordinate-wise product of matrices." (mm-apply *))

(defn m*s "The product of a matrix by scalars." [m & scalars] (mapv #(v*s % (apply * scalars)) m))
(defn m*v "The product of the matrix by the vector." [m v] (mapv (partial scalar v) m))
(defn transpose "Matrix transposition." [m] (apply mapv vector m))
(defn m*m "The product of matrices." [& matrices] (reduce #(transpose (mapv (partial m*v %1) (transpose %2))) matrices))

(defn shapeless [f]
  (fn r [& elements]
    {:pre [(or (every? number? elements) (and (every? vector? elements) (check-dimensions elements)))]}
    (if (every? number? elements) (apply f elements) (apply mapv r elements))))

(defn form-tensor? [& elements]
  (or
    (every? number? elements)
    (and (every? vector? elements) (check-dimensions elements) (apply form-tensor? (apply concat elements)))))

(defn shape-of [tensor] (loop [axis tensor result []] (if (number? axis) result (recur (first axis) (conj result (count axis))))))
(defn suffix? [v1 v2] "If v1 suffix of v2?" (and (<= (count v1) (count v2)) (= v1 (subvec v2 (- (count v2) (count v1))))))

(defn broadcast [s t]
  {:pre [(suffix? (shape-of t) s)]}
  (loop [dims (subvec s 0 (- (count s) (count (shape-of t)))) result t]
    (if (empty? dims) result (recur (pop dims) (into [] (repeat (last dims) result))))))

(defn get-longest [& vectors]
  (loop [result [] vectors vectors]
    (if (empty? vectors)
      result
      (recur (if (> (count (first vectors)) (count result)) (first vectors) result) (rest vectors)))))

(defn bc-function [f]
  (fn [& tensors] {:pre [(every? form-tensor? tensors)]}
    (apply (shapeless f) (mapv (partial broadcast (apply get-longest (mapv shape-of tensors))) tensors))))
(def b+ (bc-function +))
(def b- (bc-function -))
(def b* (bc-function *))
(def bd (bc-function /))