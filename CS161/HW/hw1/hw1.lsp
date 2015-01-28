; TREE-CONTAINS: Takes a number 'N' and ordered tree 'TREE'. Returns 't' if N
; appears in TREE.
; Base case: TREE is a number (or empty).
;			 If N == NULL, return false.
;			 If N == TREE, return true.
; Case: TREE is a list. Check left/right sub-tree.
(defun TREE-CONTAINS (N TREE)
	(cond ((null TREE) nil)
	      ((numberp TREE) (cond ((equal N TREE) t)
	      						(t nil) ))
	      ((listp TREE) (cond ((TREE-CONTAINS N (first TREE)) t)
	                          (t (TREE-CONTAINS N (rest TREE)) ) ))))

; TREE-MAX: Takes one argument, an ordered tree 'TREE', and returns the maximum 
; number appearing in 'TREE'.
; Base case: TREE is a number (or empty).
; 			 If NULL, return nil.
;			 If NUMBER, return TREE.
; Case: TREE is a list. Extract right-most number in right sub-tree.
(defun TREE-MAX (TREE)
	(cond ((null TREE) 0)
		  ((numberp TREE) TREE)
		  ((listp TREE) (cond ((>= (TREE-MAX (first TREE)) (TREE-MAX (rest TREE)) ) 
		  						   (TREE-MAX (first TREE)) )
		  					  (t (TREE-MAX (rest TREE))) ))))

; TREE-ORDER: Takes one argument, an ordered tree 'TREE', and returns an 
; in-ordered list of the numbers appearing in tree 'TREE'.
; Base case: If TREE is null, return 0.
;			If TREE is a single number, return the number as a list.
; Case: If TREE is a list, go through the list and append the first to rest.
(defun TREE-ORDER (TREE)
	(cond ((null TREE) '())
		  ((numberp TREE) (cons TREE nil))
		  ((listp TREE) (append (TREE-ORDER (first TREE)) (TREE-ORDER (rest TREE) 
		  )) )))

; SUB-LIST: Takes three arguments, a list L and two non-negative integers START 
; and LEN and returns the sub-list of L starting at position START and having 
; length LEN. 
; Base case: L is empty or LEN is 0, return nil.
; 			 or if START > LEN, return nil.
; Case: Recursive call that iterates via first, START, and LEN.
(defun SUB-LIST (L START LEN)
    (cond ((NULL L) nil)
          ((equal LEN 0) nil)
          ((atom L) L)
          ((and (equal START 0) (> LEN 0)) (cons (first L) (SUB-LIST (rest L) START (- LEN 1)) ))
          ((> START 0) (SUB-LIST (rest L) (- START 1) LEN))
          ((> START (- (length L) 1) ) nil)))

; SPLIT-LIST: Takes one argument,a list L and returns a list of two lists, L1 
; and L2 (in order) such that L is the result of appending L1 and L2 and length 
; of L2 minus length of L1 is 0 or 1.
; Uses length to check the length of the list. Uses SUB-LIST to split list.
(defun SPLIT-LIST (L)
  (cond ((evenp (length L)) (list (SUB-LIST L 0 (/ (length L) 2)) (SUB-LIST L (/ (length L) 2) (length L))))
        ((oddp (length L)) (list (SUB-LIST L 0 (/ (- (length L) 1) 2)) (SUB-LIST L (/ (- (length L) 1) 2) (length L))))))

; LIST2BTREE: Takes one argument, a non-empty list of atoms LEAVES, and returns
; a binary tree such that the tree leaves are the elements of LEAVES, and for
; any non-leaf node in the tree, the number of leaves in its right branch minus 
; the number of leaves in its left branch is 0 or 1.
(defun LIST2BTREE ( LEAVES )
  (cond ((NULL LEAVES) nil)
        ((atom LEAVES) LEAVES)
        ((equal (length LEAVES) 1) (first LEAVES))
        ((equal (length LEAVES) 2) LEAVES)
        ((equal (length LEAVES) 3) (append (first (SPLIT-LIST LEAVES)) (rest (SPLIT-LIST LEAVES))))
        ((equal (length LEAVES) 4) (SPLIT-LIST LEAVES))
        ( t (list (LIST2BTREE (first (SPLIT-LIST LEAVES))) (LIST2BTREE (second (SPLIT-LIST LEAVES)))))
  )
)

; BTREE2LIST: Takes one argument, a binary tree TREE, and returns a list of 
; atoms. BTREE2LIST is the inverse of LIST2BTREE (above).
(defun BTREE2LIST (TREE)
  (cond ((NULL TREE) '())
        ((atom TREE) (cons TREE '()))
        (t (append (BTREE2LIST (first TREE)) (BTREE2LIST (second TREE))))))
