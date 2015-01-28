;
; CS161 Hw3: Sokoban
; 
; *********************
;    READ THIS FIRST
; ********************* 
;
; All functions that you need to modify are marked with 'EXERCISE' in their header comments.
; Do not modify a-star.lsp.
; This file also contains many helper functions. You may call any of them in your functions.
;
; *Warning*: The provided A* code only supports the maximum cost of 4999 for any node.
; That is f(n)=g(n)+h(n) < 5000. So, be careful when you write your heuristic functions.
; Do not make them return anything too large.
;
; For Allegro Common Lisp users: The free version of Allegro puts a limit on memory.
; So, it may crash on some hard sokoban problems and there is no easy fix (unless you buy 
; Allegro). 
; Of course, other versions of Lisp may also crash if the problem is too hard, but the amount
; of memory available will be relatively more relaxed.
; Improving the quality of the heuristic will mitigate this problem, as it will allow A* to
; solve hard problems with fewer node expansions.
; 
; In either case, this limitation should not significantly affect your grade.
; 
; Remember that most functions are not graded on efficiency (only correctness).
; Efficiency can only influence your heuristic performance in the competition (which will
; affect your score).
;  
;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; General utility functions
; They are not necessary for this homework.
; Use/modify them for your own convenience.
;

;
; For reloading modified code.
; I found this easier than typing (load "filename") every time. 
;
(defun reload()
  (load "hw3.lsp")
  )

;
; For loading a-star.lsp.
;
(defun load-a-star()
  (load "a-star.lsp"))

;
; Reloads hw3.lsp and a-star.lsp
;
(defun reload-all()
  (reload)
  (load-a-star)
  )

;
; A shortcut function.
; goal-test and next-states stay the same throughout the assignment.
; So, you can just call (sokoban <init-state> #'<heuristic-name>).
; 
;
(defun sokoban (s h)
  (a* s #'goal-test #'next-states h)
  )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; end general utility functions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; We now begin actual Sokoban code
;

; Define some global variables
(setq blank 0)
(setq wall 1)
(setq box 2)
(setq keeper 3)
(setq star 4)
(setq boxstar 5)
(setq keeperstar 6)

; Some helper functions for checking the content of a square
(defun isBlank (v)
  (= v blank)
  )

(defun isWall (v)
  (= v wall)
  )

(defun isBox (v)
  (= v box)
  )

(defun isKeeper (v)
  (= v keeper)
  )

(defun isStar (v)
  (= v star)
  )

(defun isBoxStar (v)
  (= v boxstar)
  )

(defun isKeeperStar (v)
  (= v keeperstar)
  )

;
; Helper function of getKeeperPosition
;
(defun getKeeperColumn (r col)
  (cond ((null r) nil)
	(t (if (or (isKeeper (car r)) (isKeeperStar (car r)))
	       col
	     (getKeeperColumn (cdr r) (+ col 1))
	     );end if
	   );end t
	);end cond
  )

;
; getKeeperPosition (s firstRow)
; Returns a list indicating the position of the keeper (c r).
; 
; Assumes that the keeper is in row >= firstRow.
; The top row is the zeroth row.
; The first (right) column is the zeroth column.
;
(defun getKeeperPosition (s row)
  (cond ((null s) nil)
	(t (let ((x (getKeeperColumn (car s) 0)))
	     (if x
		 ;keeper is in this row
		 (list x row)
		 ;otherwise move on
		 (getKeeperPosition (cdr s) (+ row 1))
		 );end if
	       );end let
	 );end t
	);end cond
  );end defun

;
; cleanUpList (l)
; returns l with any NIL element removed.
; For example, if l is '(1 2 NIL 3 NIL), returns '(1 2 3).
;
(defun cleanUpList (L)
  (cond ((null L) nil)
	(t (let ((cur (car L))
		 (res (cleanUpList (cdr L)))
		 )
	     (if cur 
		 (cons cur res)
		  res
		 )
	     );end let
	   );end t
	);end cond
  );end 

; EXERCISE: Modify this function to return true (t)
; if and only if s is a goal state of a Sokoban game.
; (no box is on a non-goal square)
;
; Currently, it always returns NIL. If A* is called with
; this function as the goal testing function, A* will never
; terminate until the whole search space is exhausted.
;
; GOAL-TEST: Takes a state as the argument and returns true (t) if and only if the state is a goal state of the game. 
; Goal state will have no 2s and should have at least one 5 (but we don't need to check this).
; Parse state lists to check for any 2s. If any 2s are found, return false.
; base case: null S, return true.
; recursive cases: if first S has any 2's, return false.
; 				   else, check rest S for 2's.
; If S is a list of integers, check S for 2's.
; If S is a list of lists, check first S for 2's.
(defun goal-test (S)
	(cond ((null S) t)
		  ((atom (first S)) (cond ((> (count 2 S) 0) nil)))
		  ((listp (first S))(cond ((> (count 2 (first S)) 0) nil)
		  				    (t (goal-test (rest S)))))))


; HELPER FUNCTIONS for next-states

; GET-SQUARE: Takes state S, row r, col c, where r, c are
; integers and S is a list of lists of integers, returns 
; value of square at pos (r, c). 
; 	Base cases: 
; 			   S == null, return 1
; 	Recursive cases:
;			   r = 0, c = 0: Check S: If S is an atom
;									     make sure it's not nil.
;									  If S is a list, recurse on first S.
;			   else, use nthcdr to extract list at row = r.
 (defun get-square( S r c )
	(cond ((equal S '()) 1)
		  ((and (equal r 0) (equal c 0)) (cond ((atom S) 
		  								 	   (cond ((equal S nil) 1)
		  									   		 (t S)))
		 									   ((listp S) (get-square (first (first S)) 0 0))))
		  (t (get-square (first (nthcdr c (first (nthcdr r S)))) 0 0))))

; SET-SQUARE: Takes a state S, a row number r, a column number c, and a square content v (integer). 
; 			  Returns a new state S’ that is obtained by setting the square (r,c) to value v. 
; 	case 1: S is a state
;			extract elements before row r.
;  			extract elements after row r.
; 			cons elements after row r with new list.
; 			append first elements to list.
; 	case 2: S is a list of atoms:
;  			extract elements before col c.
;  			extract elements after col c.
; 			cons elements after col c with v.
; 			append first elements to list.

(defun set-square (S r c v)
	(cond ((listp (first S))(cond ((equal (get-square S r c) v) S)
								  (( > r (- (length S) 1)) S)
		  					 	  (t (append (butlast S (- (length S) r)) 
		  					 	  	 (cons (set-square (first (nthcdr r S)) 0 c v) (rest (nthcdr r S)))))))
		  ((atom (first S)) (cond ((> c (- (length S) 1)) S)
		  						  (t (append (butlast S (- (length S) c)) 
		  						  	 (cons v (nthcdr (+ c 1) S))))))))


; TRY-MOVE: Takes state S and direction D and returns the state
; that is that results from moving the keeper in state S in
; direction D.

; The directions are up, dn, lt, rt.

; The outermost condition checks for direction.
; e.g., If D is up, the next condition checks
; for a wall:
; 	If there is a wall, return nil.
; 	Else, check if square above is a box or 
; 		  a goal+box.
;   	  If 2 squares above the keeper is empty or
;		  a goal box, push box up.
; If the adjacent square is a box, goal+box, or
; blank, the function makes the appropriate moves 
; and picks the correct symbols for the new state.
 
; This function has been broken down into several
; helper functions to avoid redundant code and
; assist with pushing boxes.

(defun push-up-helper (S r c)
	(let* ( (2up (get-square S (- r 2) c))
			(1up (get-square S (- r 1) c))
			(cur (get-square S r c))
			(S1 (if (equal 2up 4) ; If the square two up is a goal,
					(set-square S (- r 2) c (+ 1 2up)) ; Change it to be 5
					(set-square S (- r 2) c (+ 2 2up)) ; Else, set it to 2.
				))
			(S2 (if (equal 1up 2) ; If the square one box up is a box
					(set-square S1 (- r 1) c 3) ; Move the keeper to it.
					(set-square S1 (- r 1) c 6) ; Else, it's a goal+box.
					))
			(S3 (if (equal cur 6)
					(set-square S2 r c (- cur 2))
					(set-square S2 r c (- cur 3))
					))
			) S3 )
	)

(defun up-helper (S r c)
	(let* ( (1up (get-square S (- r 1) c))
			(cur (get-square S r c))
			(S1 (if (equal 1up 4);  If the square 1 up is a goal,
					(set-square S (- r 1) c (+ 1up 2)) ; Change it to be 5
					(set-square S (- r 1) c (+ 1up 3)))) ; Else, set it to 2.
			(S2 (if (equal cur 6)
					(set-square S1 r c (- cur 2))
					(set-square S1 r c (- cur 3))))
		  ) S2 )
	)

(defun push-dn-helper (S r c)
	(let* ( (2dn (get-square S (+ r 2) c))
			(1dn (get-square S (+ r 1) c))
			(cur (get-square S r c))
			(S1 (if (equal 2dn 4) ; If the square two boxes down is a goal,
					(set-square S (+ r 2) c (+ 1 2dn)) ; Change it to be 5
					(set-square S (+ r 2) c (+ 2 2dn)) ; Else, set it to 2.
				))
			(S2 (if (equal 1dn 2) ; If the square below is a box
					(set-square S1 (+ r 1) c 3) ; Change it to the keeper
					(set-square S1 (+ r 1) c 6) ; Else, keeper+box
					))
			(S3 (if (equal cur 6)
					(set-square S2 r c (- cur 2))
					(set-square S2 r c (- cur 3))
					))
			) S3 )
	)

(defun dn-helper (S r c)
	(let* ( (1dn (get-square S (+ r 1) c))
			(cur (get-square S r c))
			(S1 (if (equal 1dn 4) ; If the square one box down is a goal
					(set-square S (+ r 1) c (+ 1dn 2)) ; Change it to 6
					(set-square S (+ r 1) c (+ 1dn 3)))) ; Else, set it to 3.
			(S2 (if (equal cur 6)
					(set-square S1 r c (- cur 2))
					(set-square S1 r c (- cur 3))))
		  ) S2 )
	)

(defun push-rt-helper (S r c)
	(let* ( (2rt (get-square S r (+ c 2)))
			(1rt (get-square S r (+ c 1)))
			(cur (get-square S r c))
			(S1 (if (equal 2rt 4) ; If the square two boxes rt is a goal,
					(set-square S r (+ c 2) (+ 1 2rt)) ; Change it to be 5
					(set-square S r (+ c 2) (+ 2 2rt)) ; Else, set it to 2.
				))
			(S2 (if (equal 1rt 2)
					(set-square S1 r (+ c 1) 3)
					(set-square S1 r (+ c 1) 6)
					))
			(S3 (if (equal cur 6)
					(set-square S2 r c (- cur 2))
					(set-square S2 r c (- cur 3))
					))
			) S3 )
	)

(defun rt-helper (S r c)
	(let* ( (1rt (get-square S r (+ c 1)))
			(cur (get-square S r c))
			(S1 (if (equal 1rt 4)
					(set-square S r (+ c 1) (+ 1rt 2))
					(set-square S r (+ c 1) (+ 1rt 3))))
			(S2 (if (equal cur 6)
					(set-square S1 r c (- cur 2))
					(set-square S1 r c (- cur 3))))
		  ) S2 )
	)

(defun push-lt-helper (S r c)
	(let* ( (2lt (get-square S r (- c 2)))
			(1lt (get-square S r (- c 1)))
			(cur (get-square S r c))
			(S1 (if (equal 2lt 4) ; If the square two boxes lt is a goal,
					(set-square S r (- c 2) (+ 1 2lt)) ; Change it to be 5
					(set-square S r (- c 2) (+ 2 2lt)) ; Else, set it to 2.
				))
			(S2 (if (equal 1lt 2)
					(set-square S1 r (- c 1) 3)
					(set-square S1 r (- c 1) 6)
					))
			(S3 (if (equal cur 6)
					(set-square S2 r c (- cur 2))
					(set-square S2 r c (- cur 3))
					))
			) S3 )
	)

(defun lt-helper (S r c)
	(let* ( (1lt (get-square S r (- c 1)))
			(cur (get-square S r c))
			(S1 (if (equal 1lt 4)
					(set-square S r (- c 1) (+ 1lt 2))
					(set-square S r (- c 1) (+ 1lt 3))))
			(S2 (if (equal cur 6)
					(set-square S1 r c (- cur 2))
					(set-square S1 r c (- cur 3))))
		  ) S2 )
	)


(defun try-move (S D)
	(let ( (c (first (getkeeperposition S 0)))
		   (r (second (getkeeperposition S 0))) )
		(cond ((equal D 'up) (cond ((equal (get-square S (- r 1) c) 1) nil)
								   ((or (equal (get-square S (- r 1) c) 2) (equal (get-square S (- r 1) c) 5)) ; If the sq above is a 2 or 5
									   	(cond ((or (equal (get-square S (- r 2) c) 0) 
									   			   (equal (get-square S (- r 2) c) 4)) 
									   			   (push-up-helper S r c)) ; If the sq above that is blank or goal
									   		  (t nil)))
								   (t (up-helper S r c))))
				((equal D 'dn) (cond ((equal (get-square S (+ r 1) c) 1) nil)
								   	 ((or (equal (get-square S (+ r 1) c) 2) (equal (get-square S (+ r 1) c) 5))
									      (cond ((or (equal (get-square S (+ r 2) c) 0) 
									   			     (equal (get-square S (+ r 2) c) 4)) 
									   			     (push-dn-helper S r c))
									   		  	(t nil)))
								   	 (t (dn-helper S r c))))
			  	((equal D 'lt) (cond ((equal (get-square S r (- c 1)) 1) nil)
								   	 ((or (equal (get-square S r (- c 1)) 2) (equal (get-square S r (- c 1)) 5))
								   		  (cond ((or (equal (get-square S r (- c 2)) 0) 
									   		  		 (equal (get-square S r (- c 2)) 4)) 
									   				 (push-lt-helper S r c))
									   		 	(t nil)))
									 (t (lt-helper S r c))))
			  	((equal D 'rt) (cond ((equal (get-square S r (+ c 1)) 1) nil)
								   	 ((or (equal (get-square S r (+ c 1)) 2) (equal (get-square S r (+ c 1)) 5))
								   		  (cond ((or (equal (get-square S r (+ c 2)) 0) 
									   		  		 (equal (get-square S r (+ c 2)) 4)) 
									   				 (push-rt-helper S r c))
									   		 	(t nil)))
								   	 (t (rt-helper S r c)))))))

; EXERCISE: Modify this function to return the list of 
; sucessor states of s.
;
; This is the top-level next-states (successor) function.
; Some skeleton code is provided below.
; You may delete them totally, depending on your approach.
; 
; If you want to use it, you will need to set 'result' to be 
; the set of states after moving the keeper in each of the 4 directions.
; A pseudo-code for this is:
; 
; ...
; (result (list (try-move s UP) (try-move s DOWN) (try-move s LEFT) (try-move s RIGHT)))
; ...
; 
; You will need to define the function try-move and decide how to represent UP,DOWN,LEFT,RIGHT.
; Any NIL result returned from try-move can be removed by cleanUpList.

(defun next-states (s)
  (let* ((pos (getKeeperPosition s 0))
	 (x (car pos))
	 (y (cadr pos))
	 ;x and y are now the coordinate of the keeper in s.
	 (result (list (try-move s 'up) (try-move s 'dn) (try-move s 'lt) (try-move s 'rt) ) ) )
    (cleanUpList result);end
   );end let
  )

; EXERCISE: Modify this function to compute the trivial 
; admissible heuristic.
;
(defun h0 (s)
  '0 )


; EXERCISE: Modify this function to compute the 
; number of misplaced boxes in s.
; H1: A heuristic that takes one argument, a state
; s, and returns the number of misplaced boxes in s. 
; H1 checks the count of 2's (boxes not on goals) in 
; each sublist by using count on (first s) and then
; recursing on (rest s).
; H1 is an admissible heuristic. Since there are at
; least as many goal squares as boxes, any misplaced
; box has a goal square it can be placed on. Thus, 
; every misplaced box will require at least one move
; to reach a goal square.
(defun h1 (s)
	(cond ((null S) 0)
		  (t (+ (count 2 (first s)) (h1 (rest S)))))
  )

; HELPER FUNCTIONS FOR H704068040
; Computes minimum distance from object at position pos
; to nearest goal square.
(defun minDistToGoal(s r c)
	(let* ( (gpos (getGoalPos s 0)); If a goal is found, 
		  		    (gr (second gpos)); get the position of the goal,
		  		    (gc (first gpos))
		  	)
			(cond ((null s) 0)
				  ((or (null r) (null gr) (null c) (null gc)) 0)
				  (t (let* ((smod (set-square s gr gc 0)); modify s so that goal = 0 before recursing
				  		    (min1 (calcDist r gr c gc))  ; to avoid double counting goal squares.
				  		    (min3 (minDistToGoal smod r c))
				  		    (min2 (if (equal min3 0) ; Dealing with 0s when recursion
				  		    		   (+ min1 min3) ; bottoms out
				  		    		   min3)); look for a shorter distance
				  		    )
				  		(if (<= min1 min2)
				  			min1
				  			min2
				  		)
				  	 )
				  )
			)
	)
)

; Computes the horizontal + vertical distance between
; arguments (r1 c1) and (r2 c2), which represent coords
; in the Sokoban game.
(defun calcDist (r1 r2 c1 c2)
	(cond ((or (null r1) (null r2) (null c1) (null c2)) 0)
		  (t (let ( (vdist (if (< (- r2 r1) 0) 
							   (* -1 (- r2 r1))
					 		   (- r2 r1)))
		 			 (hdist (if (< (- c2 c1) 0) 
					 			(* -1 (- c2 c1))
					 			(- c2 c1)))
		  		  )
				  (+ hdist vdist)
			)
		  )
	)
)

; Helper function for getBoxPos
(defun getBoxCol (r col)
  (cond ((null r) nil)
	(t (if (or (isBox (car r)) (isBoxStar (car r)))
	       col
	     (getBoxCol (cdr r) (+ col 1))
	     );end if
	   );end t
	);end cond
  )

; Helper function that returns position of
; first box on Sokoban grid.
(defun getBoxPos(s row)
  (cond ((null s) nil)
	(t (let ((x (getBoxCol (car s) 0)))
	     (if x
		 ;keeper is in this row
		 (list x row)
		 ;otherwise move on
		 (getBoxPos (cdr s) (+ row 1))
		 );end if
	       );end let
	 );end t
	);end cond
  );end defun

; Helper function for getGoalPos	  		  
(defun getGoalCol (r col)
  (cond ((null r) nil)
	(t (if (or (isStar (car r)) (isKeeperStar (car r)))
	       col
	     (getGoalCol (cdr r) (+ col 1))
	     );end if
	   );end t
	);end cond
  )

; Helper function that returns position of
; first goal on Sokoban grid.
(defun getGoalPos(s row)
  (cond ((null s) nil)
	(t (let ((x (getGoalCol (car s) 0)))
	     (if x
		 ;keeper is in this row
		 (list x row)
		 ;otherwise move on
		 (getGoalPos (cdr s) (+ row 1))
		 );end if
	       );end let
	 );end t
	);end cond
  );end defun

; EXERCISE: Change the name of this function to h<UID> where
; <UID> is your actual student ID number. Then, modify this 
; function to compute an admissible heuristic value of s. 
; 
; This function will be entered in the competition.
; Objective: make A* solve problems as fast as possible.
; The Lisp 'time' function can be used to measure the 
; running time of a function call.

; H704068040: A heuristic function that takes in one argument,
; a state s, and returns the summed vertical + horizontal
; distances of boxes from goals. Since there can be more 
; goals than boxes, the fn calculates the MIN distance of a 
; box from any empty goal square.
; Need to find box, and then find goal. First parse state
; for box, then parse state for nearest goal. Replace box
; with blank after min goal distance computed.
; Repeat until end of list or no more boxes.
(defun h704068040 (s)
	(let* ((pos (getBoxPos s 0)); If a box is found,
		  (r (second pos)); get the row of the box,
		  (c (first pos)); and col of the box
		  )
		(cond ((null s) 0)
			  ((null r) 0)
		  	  ((null c) 0)
			  (t (+ (minDistToGoal s r c) (h2 (set-square s r c 0))))); add distance computed by minDistToGoal to total.
	)
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#|
 | Some predefined problems.
 | Each problem can be visualized by calling (printstate <problem>). For example, (printstate p1).
 | Problems are ordered roughly by their difficulties.
 | For most problems, we also privide 2 additional number per problem:
 |    1) # of nodes expanded by A* using our next-states and h0 heuristic.
 |    2) the depth of the optimal solution.
 | These numbers are located at the comments of the problems. For example, the first problem below 
 | was solved by 80 nodes expansion of A* and its optimal solution depth is 7.
 | 
 | Your implementation may not result in the same number of nodes expanded, but it should probably
 | give something in the same ballpark. As for the solution depth, any admissible heuristic must 
 | make A* return an optimal solution. So, the depths of the optimal solutions provided could be used
 | for checking whether your heuristic is admissible.
 |
 | Warning: some problems toward the end are quite hard and could be impossible to solve without a good heuristic!
 | 
 |#

;(80,7)
(setq p1 '((1 1 1 1 1 1)
	   (1 0 3 0 0 1)
	   (1 0 2 0 0 1)
	   (1 1 0 1 1 1)
	   (1 0 0 0 0 1)
	   (1 0 0 0 4 1)
	   (1 1 1 1 1 1)))

;(110,10)
(setq p2 '((1 1 1 1 1 1 1)
	   (1 0 0 0 0 0 1) 
	   (1 0 0 0 0 0 1) 
	   (1 0 0 2 1 4 1) 
	   (1 3 0 0 1 0 1)
	   (1 1 1 1 1 1 1)))

;(211,12)
(setq p3 '((1 1 1 1 1 1 1 1 1)
	   (1 0 0 0 1 0 0 0 1)
	   (1 0 0 0 2 0 3 4 1)
	   (1 0 0 0 1 0 0 0 1)
	   (1 0 0 0 1 0 0 0 1)
	   (1 1 1 1 1 1 1 1 1)))

;(300,13)
(setq p4 '((1 1 1 1 1 1 1)
	   (0 0 0 0 0 1 4)
	   (0 0 0 0 0 0 0)
	   (0 0 1 1 1 0 0)
	   (0 0 1 0 0 0 0)
	   (0 2 1 0 0 0 0)
	   (0 3 1 0 0 0 0)))

;(551,10)
(setq p5 '((1 1 1 1 1 1)
	   (1 1 0 0 1 1)
	   (1 0 0 0 0 1)
	   (1 4 2 2 4 1)
	   (1 0 0 0 0 1)
	   (1 1 3 1 1 1)
	   (1 1 1 1 1 1)))

;(722,12)
(setq p6 '((1 1 1 1 1 1 1 1)
	   (1 0 0 0 0 0 4 1)
	   (1 0 0 0 2 2 3 1)
	   (1 0 0 1 0 0 4 1)
	   (1 1 1 1 1 1 1 1)))

;(1738,50)
(setq p7 '((1 1 1 1 1 1 1 1 1 1)
	   (0 0 1 1 1 1 0 0 0 3)
	   (0 0 0 0 0 1 0 0 0 0)
	   (0 0 0 0 0 1 0 0 1 0)
	   (0 0 1 0 0 1 0 0 1 0)
	   (0 2 1 0 0 0 0 0 1 0)
	   (0 0 1 0 0 0 0 0 1 4)))

;(1763,22)
(setq p8 '((1 1 1 1 1 1)
	   (1 4 0 0 4 1)
	   (1 0 2 2 0 1)
	   (1 2 0 1 0 1)
	   (1 3 0 0 4 1)
	   (1 1 1 1 1 1)))

;(1806,41)
(setq p9 '((1 1 1 1 1 1 1 1 1) 
	   (1 1 1 0 0 1 1 1 1) 
	   (1 0 0 0 0 0 2 0 1) 
	   (1 0 1 0 0 1 2 0 1) 
	   (1 0 4 0 4 1 3 0 1) 
	   (1 1 1 1 1 1 1 1 1)))

;(10082,51)
(setq p10 '((1 1 1 1 1 0 0)
	    (1 0 0 0 1 1 0)
	    (1 3 2 0 0 1 1)
	    (1 1 0 2 0 0 1)
	    (0 1 1 0 2 0 1)
	    (0 0 1 1 0 0 1)
	    (0 0 0 1 1 4 1)
	    (0 0 0 0 1 4 1)
	    (0 0 0 0 1 4 1)
	    (0 0 0 0 1 1 1)))

;(16517,48)
(setq p11 '((1 1 1 1 1 1 1)
	    (1 4 0 0 0 4 1)
	    (1 0 2 2 1 0 1)
	    (1 0 2 0 1 3 1)
	    (1 1 2 0 1 0 1)
	    (1 4 0 0 4 0 1)
	    (1 1 1 1 1 1 1)))

;(22035,38)
(setq p12 '((0 0 0 0 1 1 1 1 1 0 0 0)
	    (1 1 1 1 1 0 0 0 1 1 1 1)
	    (1 0 0 0 2 0 0 0 0 0 0 1)
	    (1 3 0 0 0 0 0 0 0 0 0 1)
	    (1 0 0 0 2 1 1 1 0 0 0 1)
	    (1 0 0 0 0 1 0 1 4 0 4 1)
	    (1 1 1 1 1 1 0 1 1 1 1 1)))

;(26905,28)
(setq p13 '((1 1 1 1 1 1 1 1 1 1)
	    (1 4 0 0 0 0 0 2 0 1)
	    (1 0 2 0 0 0 0 0 4 1)
	    (1 0 3 0 0 0 0 0 2 1)
	    (1 0 0 0 0 0 0 0 0 1)
	    (1 0 0 0 0 0 0 0 4 1)
	    (1 1 1 1 1 1 1 1 1 1)))

;(41715,53)
(setq p14 '((0 0 1 0 0 0 0)
	    (0 2 1 4 0 0 0)
	    (0 2 0 4 0 0 0)	   
	    (3 2 1 1 1 0 0)
	    (0 0 1 4 0 0 0)))

;(48695,44)
(setq p15 '((1 1 1 1 1 1 1)
	    (1 0 0 0 0 0 1)
	    (1 0 0 2 2 0 1)
	    (1 0 2 0 2 3 1)
	    (1 4 4 1 1 1 1)
	    (1 4 4 1 0 0 0)
	    (1 1 1 1 0 0 0)
	    ))

;(91344,111)
(setq p16 '((1 1 1 1 1 0 0 0)
	    (1 0 0 0 1 0 0 0)
	    (1 2 1 0 1 1 1 1)
	    (1 4 0 0 0 0 0 1)
	    (1 0 0 5 0 5 0 1)
	    (1 0 5 0 1 0 1 1)
	    (1 1 1 0 3 0 1 0)
	    (0 0 1 1 1 1 1 0)))

;(3301278,76)
(setq p17 '((1 1 1 1 1 1 1 1 1 1)
	    (1 3 0 0 1 0 0 0 4 1)
	    (1 0 2 0 2 0 0 4 4 1)
	    (1 0 2 2 2 1 1 4 4 1)
	    (1 0 0 0 0 1 1 4 4 1)
	    (1 1 1 1 1 1 0 0 0 0)))

;(??,25)
(setq p18 '((0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
	    (0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
	    (1 1 1 1 1 0 0 0 0 0 0 1 1 1 1 1)
	    (0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0)
	    (0 0 0 0 0 0 1 0 0 1 0 0 0 0 0 0)
	    (0 0 0 0 0 0 0 0 3 0 0 0 0 0 0 0)
	    (0 0 0 0 0 0 1 0 0 1 0 0 0 0 0 0)
	    (0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0)
	    (1 1 1 1 1 0 0 0 0 0 0 1 1 1 1 1)
	    (0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
	    (0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
	    (0 0 0 0 1 0 0 0 0 0 4 1 0 0 0 0)
	    (0 0 0 0 1 0 2 0 0 0 0 1 0 0 0 0)	    
	    (0 0 0 0 1 0 2 0 0 0 4 1 0 0 0 0)
	    ))
;(??,21)
(setq p19 '((0 0 0 1 0 0 0 0 1 0 0 0)
	    (0 0 0 1 0 0 0 0 1 0 0 0)
	    (0 0 0 1 0 0 0 0 1 0 0 0)
	    (1 1 1 1 0 0 0 0 1 1 1 1)
	    (0 0 0 0 1 0 0 1 0 0 0 0)
	    (0 0 0 0 0 0 3 0 0 0 2 0)
	    (0 0 0 0 1 0 0 1 0 0 0 4)
	    (1 1 1 1 0 0 0 0 1 1 1 1)
	    (0 0 0 1 0 0 0 0 1 0 0 0)
	    (0 0 0 1 0 0 0 0 1 0 0 0)
	    (0 0 0 1 0 2 0 4 1 0 0 0)))

;(??,??)
(setq p20 '((0 0 0 1 1 1 1 0 0)
	    (1 1 1 1 0 0 1 1 0)
	    (1 0 0 0 2 0 0 1 0)
	    (1 0 0 5 5 5 0 1 0)
	    (1 0 0 4 0 4 0 1 1)
	    (1 1 0 5 0 5 0 0 1)
	    (0 1 1 5 5 5 0 0 1)
	    (0 0 1 0 2 0 1 1 1)
	    (0 0 1 0 3 0 1 0 0)
	    (0 0 1 1 1 1 1 0 0)))

;(??,??)
(setq p21 '((0 0 1 1 1 1 1 1 1 0)
	    (1 1 1 0 0 1 1 1 1 0)
	    (1 0 0 2 0 0 0 1 1 0)
	    (1 3 2 0 2 0 0 0 1 0)
	    (1 1 0 2 0 2 0 0 1 0)
	    (0 1 1 0 2 0 2 0 1 0)
	    (0 0 1 1 0 2 0 0 1 0)
	    (0 0 0 1 1 1 1 0 1 0)
	    (0 0 0 0 1 4 1 0 0 1)
	    (0 0 0 0 1 4 4 4 0 1)
	    (0 0 0 0 1 0 1 4 0 1)
	    (0 0 0 0 1 4 4 4 0 1)
	    (0 0 0 0 1 1 1 1 1 1)))

;(??,??)
(setq p22 '((0 0 0 0 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0)
	    (0 0 0 0 1 0 0 0 1 0 0 0 0 0 0 0 0 0 0)
	    (0 0 0 0 1 2 0 0 1 0 0 0 0 0 0 0 0 0 0)
	    (0 0 1 1 1 0 0 2 1 1 0 0 0 0 0 0 0 0 0)
	    (0 0 1 0 0 2 0 2 0 1 0 0 0 0 0 0 0 0 0)
	    (1 1 1 0 1 0 1 1 0 1 0 0 0 1 1 1 1 1 1)
	    (1 0 0 0 1 0 1 1 0 1 1 1 1 1 0 0 4 4 1)
	    (1 0 2 0 0 2 0 0 0 0 0 0 0 0 0 0 4 4 1)
	    (1 1 1 1 1 0 1 1 1 0 1 3 1 1 0 0 4 4 1)
	    (0 0 0 0 1 0 0 0 0 0 1 1 1 1 1 1 1 1 1)
	    (0 0 0 0 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#|
 | Utility functions for printing states and moves.
 | You do not need to understand any of the functions below this point.
 |#

;
; Helper function of prettyMoves
; from s1 --> s2
;
(defun detectDiff (s1 s2)
  (let* ((k1 (getKeeperPosition s1 0))
	 (k2 (getKeeperPosition s2 0))
	 (deltaX (- (car k2) (car k1)))
	 (deltaY (- (cadr k2) (cadr k1)))
	 )
    (cond ((= deltaX 0) (if (> deltaY 0) 'DOWN 'UP))
	  (t (if (> deltaX 0) 'RIGHT 'LEFT))
	  );end cond
    );end let
  );end defun

;
; Translates a list of states into a list of moves.
; Usage: (prettyMoves (a* <problem> #'goal-test #'next-states #'heuristic))
;
(defun prettyMoves (m)
  (cond ((null m) nil)
	((= 1 (length m)) (list 'END))
	(t (cons (detectDiff (car m) (cadr m)) (prettyMoves (cdr m))))
	);end cond
  );

;
; Print the content of the square to stdout.
;
(defun printSquare (s)
  (cond ((= s blank) (format t " "))
	((= s wall) (format t "#"))
	((= s box) (format t "$"))
	((= s keeper) (format t "@"))
	((= s star) (format t "."))
	((= s boxstar) (format t "*"))
	((= s keeperstar) (format t "+"))
	(t (format t "|"))
	);end cond
  )

;
; Print a row
;
(defun printRow (r)
  (dolist (cur r)
    (printSquare cur)    
    )
  );

;
; Print a state
;
(defun printState (s)
  (progn    
    (dolist (cur s)
      (printRow cur)
      (format t "~%")
      )
    );end progn
  )

;
; Print a list of states with delay.
;
(defun printStates (sl delay)
  (dolist (cur sl)
    (printState cur)
    (sleep delay)
    );end dolist
  );end defun
