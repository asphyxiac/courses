; Discussion 2
	
	;Homework solutions:

	;; SPLIT-LIST takes a list L and returns a list of two lists L1 and L2
	;; Base case: length of list = 0, return empty list
	(defun SPLIT-LIST (L)
		(let((LEN (length L))) ;; Can also use let* here instead of nested let commands.
			(let ((L1-LEN (cond ((evenp LEN) (/ LEN 2))
								(t (/ (- LEN 1) 2)))))) ;; CASE: LEN is odd
			;; if LEN is even then LEN-L1-LEN = L1-LEN
			;; otherwise, LEN-L1-LEN = L1-LEN+1
			(list 
				(SUB-LIST L 0 L1-LEN)
				(SUB-LIST L L1-LEN (- LEN L1-LEN)))))

	;; LIST2BTREE
	(defun LIST2BTREE (LEAVES)
		(cond ((null (rest LEAVES)) (first LEAVES)) ;; base case: LEAVES is a singleton
			  (t (let ((PAIR (SPLIT-LIST LEAVES))) ;; recursive case
			  		(list
			  			(LIST2BTREE (first pair)) ;; recursing on 1st element of pair
			  			(LIST2BTREE (first (rest pair)))))))) ;; recursing on 2nd element of pair

	;; BTREE2LIST
	(defun BTREE2LIST (TREE)
		(cond
			((atom TREE) (list TREE))
			(t (append (BTREE2LIST (first TREE)) (BTREE2LIST (second TREE))))))

	;; TREE-ORDER
	(defun TREE-ORDER (TREE)
		(if (atom TREE)
			(list TREE)
			(append (TREE-ORDER (first TREE))
				(cons (second TREE))
					  (TREE-ORDER (third TREE)))))

	;; TREE-CONTAINS
	(defun TREE-CONTAINS (N TREE)
		(if atom TREE)
		(= N TREE)
		(let ((M (second TREE)))
			(cond ((= N M) t)
				  ((< N M) (TREE-CONTAINS N (first TREE)))
				  (t (TREE-CONTAINS N (third TREE))))))

	;; TREE-MAX
	(defun TREE-MAX (TREE)
		(if (atom TREE)
			TREE
			(TREE-MAX (third TREE))))

	;; SUB-LIST
	(defun SUB-LIST (L START LEN)
		(cond ((= LEN 0) nil) ;; base case
			  ((= START 0) (cons (car L) (SUB-LIST (cdr L) 0 (- LEN 1)))) ;; 1st recursive case
			  (t (SUB-LIST (cdr L) (- START 1) LEN)))) ;; 2nd recursive case


