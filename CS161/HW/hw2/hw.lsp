;args: TREE: list representation of a search tree.
;
;base case: TREE is NULL.
;       Return empty list.
;      TREE is an atom.
;       Return (list TREE).
;recursive: TREE is a list.
;       Return append (DFS((first tree)) DFS((rest TREE)))

(defun DFS (TREE)
  (cond ((null TREE) '()) 
      ((atom TREE) (list TREE))
      ((listp TREE) (append (DFS( first TREE )) (DFS( rest TREE ))))))


; Base case: depth = 0 -> return '()
; Recursive case: depth > 0
;   Case 1: First element is a list, recurse on first element at depth D-1 and rest of tree at depth D.
;   Case 2: First element is an atom, append the atomic element and recurse on rest of tree at depth D.

(defun DFID (TREE D)
  (cond ((<= D 0) '())
      ((> D 0) (append (DFID TREE (- D 1) )(LDFS TREE D)))
  ))

(defun LDFS( TREE D ) 
  (cond ((null TREE) '())
      ((<= D 0) '())
      ((> D 0) (cond ((listp (first TREE)) (append (LDFS (first TREE) (- D 1)) (LDFS (rest TREE) D)))
               ((atom (first TREE)) (append (list (first TREE)) (LDFS (rest TREE) D))))
  )))

; FINAL-STATE takes a single argument s, the current state, and returns T if it
; is the goal state (3 3 NIL) and NIL otherwise.
(defun final-state (s)
  (let (( goal '(3 3 NIL) ))
  	(cond (( equal s goal ) t)
  		  ( t nil ) )))

; NEXT-STATE returns the state that results from applying an operator to the
; current state. It takes three arguments: the current state (s), a number of
; missionaries to move (m), and a number of cannibals to move (c). It returns a
; list containing the state that results from moving that number of missionaries
; and cannibals from the current side of the river to the other side of the
; river. If applying this operator results in an invalid state (because there
; are more cannibals than missionaries on either side of the river, or because
; it would move more missionaries or cannibals than are on this side of the
; river) it returns NIL.
;
; NOTE that next-state returns a list containing the successor state (which is
; itself a list); the return should look something like ((1 1 T)).

(defun next-state (s m c)
  (let ( (sidecur (third s))
  		  (moppnxt (+ m (- 3 (first s))))
  		  (coppnxt (+ c (- 3 (second s))))
  		  (mnxt (- (first s) m))
  		  (cnxt (- (second s) c)) )
  	(cond ((and (> mnxt 0) (< mnxt cnxt )) nil)
  		  ((and (> moppnxt 0) (< moppnxt coppnxt )) nil)
  		  ((> (+ m c) 2) nil) 
  		  ((< (+ m c) 1) nil)
  		  (t (list (list moppnxt coppnxt (not sidecur)))))))


; SUCC-FN returns all of the possible legal successor states to the current
; state. It takes a single argument (s), which encodes the current state, and
; returns a list of each state that can be reached by applying legal operators
; to the current state.

(defun succ-fn (s)
 (let* ((succ-list (append (next-state s 1 1) (next-state s 1 0)))
 		(succ-list (append succ-list (next-state s 0 1)))
 		(succ-list (append succ-list (next-state s 2 0)))
 		(succ-list (append succ-list (next-state s 0 2))))
 		(append succ-list '())))
 	


; ON-PATH checks whether the current state is on the stack of states visited by
; this depth-first search. It takes two arguments: the current state (s) and the
; stack of states visited by MC-DFS (states). It returns T if s is a member of
; states and NIL otherwise.

(defun on-path (s states)
  (cond ((null states) nil)
  		((equal s (first states)) T)
  		(t (on-path s (rest states)))))

; MULT-DFS is a helper function for MC-DFS. It takes two arguments: a stack of
; states from the initial state to the current state (path), and the legal
; successor states to the last state on that stack (states). states is a
; first-in first-out list of states; that is, the first element is the initial
; state for the current search and the last element is the most recent state
; explored. MULT-DFS does a depth-first search on each element of states in
; turn. If any of those searches reaches the final state, MULT-DFS returns the
; complete path from the initial state to the goal state. Otherwise, it returns
; NIL.

(defun mult-dfs (states path)
  (if (null states) nil)
  (cond ((final-state (first states)) path)
  		((null (first states)) path)
  		(t (mult-dfs (succ-fn (first states)) (cons (first states) path)))))

; MC-DFS does a depth first search from a given state to the goal state. It
; takes two arguments: a state (S) and the path from the initial state to S
; (PATH). If S is the initial state in our search, PATH should be NIL. MC-DFS
; performs a depth-first search starting at the given state. It returns the path
; from the initial state to the goal state, if any, or NIL otherwise. MC-DFS is
; responsible for checking if S is already the goal state, as well as for
; ensuring that the depth-first search does not revisit a node already on the
; search path.

(defun mc-dfs (s path)
  (cond ((null s) nil)
  		((final-state (first s)) path)
  		((not (on-path (first s) path)) (mult-dfs s (cons (first s) path)))))