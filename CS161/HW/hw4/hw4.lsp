; absX: Takes one integer argument, x, and returns the absolute value of x.
( defun absX (x) 
	(cond ((< x 0) (* x -1)) ; If x < 0, multiply it by -1.
		  (t x) ))

; INITIALIZATION AND GOAL TEST

; isGoal: Takes one arg, current state of the board.
; Checks to see if current state is goal.
(defun isGoal (n.queens) 
	(cond ((equal (count nil n.queens) 0) t)
		  (t nil)
	)
)

; initAssign: Takes one arg, integer number of queens.
; returns N-length list of nils.
(defun initAssign(N)
	(cond ((<= N 0) '())
		  (t (append (list nil) (initAssign (- N 1))))
	)
)

; initDomain: Takes two args, integer start and integer number of queens.
; Returns n-length sequential list from start to N.
(defun initDomain(start N)
	(cond ((> start N) '())
          (t (cons start (initDomain (+ start 1) N)))
    )
 )

; computeDomain: Takes two args, current state of the board and initial domain.
; Returns new domain given current state of the board.
(defun computeDomain (n.queens domain)
	(cond ((null domain) '())
		  ((> (count (first domain) n.queens) 0) (computeDomain n.queens (rest domain)))
		  (t (append (list (first domain)) (computeDomain n.queens (rest domain))))
	)
)

; CHECKING VALIDITY OF STATES:
; Given a state, queens, need to check if any two queens are in same diagonal.
; Need to do a pairwise check of queens. Define checkDiag and helper function
; checkPairwiseDiag to achieve this task.

; checkDiag: Takes in a state, queens, and recursively checks if any two queens 
; are on the same diagonal by invoking the helper function checkPairwiseDiag.
; Returns true if no two queens share a diagonal.
(defun checkDiag (queens)
	(cond ((null queens) t) ; If the board is empty, return true.
		  ((null (first queens)) (checkDiag (rest queens))) ; If q1 is nil,
		  ; check rest of board.
		  ((null (rest queens)) t) ; If q1 is the only queen, return true.
		  ; Pairwise check of q1...qN, q2...qN, ... , qN.
		  (t (and (checkPairwiseDiag 1 (first queens) (rest queens)) 
		  		  (checkDiag (rest queens))))
	)
)
; checkPairwiseDiag: Takes three args, col := queen in 1st row (q1), 
; delta-r := row difference between queen in 1st row and queen in 2nd row (q2).
; rest-queens := remaining queens on board.
; Returns true if q1 does not share a diagonal with any other queen on board.
; q1 is on the same diagonal as q2 if delta-r = (- r1 r2) is equal to (- c1 c2 )
; OR - (- c1 c2 ).
(defun checkPairwiseDiag (delta-r col rest-queens) 
	(cond ((null rest-queens) t) ; If there are no other queens, return true.
		  ((null (first rest-queens)) ; If the adj queen is nil, check rest.
		  		 (checkPairwiseDiag (+ delta-r 1) col (rest rest-queens)))
		  ; Check q1 vs q2 and q1 vs ... qN.
		  (t (and (not (equal delta-r (absX (- col (first rest-queens)))))
		  	      (checkPairwiseDiag (+ delta-r 1) col (rest rest-queens))))
		  ))

; allDifferent: Takes one argument, the current state of the board.
; Returns true if each assigned value is different.
(defun allDifferent(n.queens)
	(cond ((null n.queens) t) ; If board empty, return true.
		  ((null (rest n.queens)) t) ; If only one queen, return true.
		  ; If first queen is nil, check rest of board.
		  ((null (first n.queens)) (allDifferent (rest n.queens)))
		  ; Else, if repeated col value, return nil.
		  ((> (count (first n.queens) (rest n.queens)) 0) nil)
		  ; Else, check rest of board.
		  (t (allDifferent (rest n.queens)))
		  ))

; isLegal: Takes one argument, the current state of the board.
; Returns true if state does not violate constraints.
(defun isLegal(n.queens) 
	(cond ((and (checkDiag n.queens) (allDifferent n.queens)) t)
		  (t nil)
	)
)

; HEURISTICS
; Need to find a way to select unassigned variable (row).
; Choose the most restricted variable (MRV), i.e., the row with
; the fewest remaining values.

; MRV HELPER FUNCTIONS

; firstUnassigned: Takes one arg, the current state of the board.
; Returns first unassigned variable (row).
(defun firstUnassigned( n.queens )
	(cond ((null (first n.queens)) 1)
		  (t (+ 1 (firstUnassigned (rest n.queens))))
	)
)

; findUnassigned: Takes three args, the current state of the board, the 
; domain, and the row to start searching at.
; Returns a list of form ((row (domain) ... (row (domain)))

; Need to find rest of the unassigned variables:
; 		Know the row of the first unassigned variable.
;		Find the row of the unassigned variables in the remaining list.
;		e.g., n.queens <- (1 2 nil nil nil 3 nil ) 
;			  		row of first unassigned variable is 3.
;			  (nthcdr 3 n.queens) returns (nil nil 3 nil)
;			 		row of next unassigned variable is row+1
(defun findUnassigned ( n.queens domain start )
	(cond ((null n.queens) '()) ; Base case: n.queens is null
		  ((> start (length domain)) '()) 
		  ; Else, get first unassigned row from firstUnassigned(n.queens)
		  (t (let* ( (row (+ start (firstUnassigned (nthcdr start n.queens))))
		  		   )
		  		(append (list (list row domain)) (findUnassigned n.queens domain row))
		  	 )
		  ))
)

; tryVar: Takes three args, the current state of the board, the variable to try,
; and its domain.
; Returns legal domain for variable (row).

; Need to try assigning unassigned variables.
; If domain(var) is empty, return empty list.
; Assign var in row first value from domain(var)
; If assignment is not legal, domain(var) = rest domain(var) (recursive call)
; If assignment is legal, domain(var) = append first element to list
; returned from recursive call.
(defun tryVar(n.queens domain row)
	(cond ((null domain) '())
		  (t (let*( (checkState (append (butlast n.queens (- (length n.queens) (- row 1))) 
		  								(append (list (first domain)) (nthcdr row n.queens))))
		  		  )
		  		  (if (not (isLegal checkState))
		  		  	  (tryVar n.queens (rest domain) row)
		  		  	  (append (list (first domain)) (tryVar n.queens (rest domain) row))
		  		  )
		  	  )
		  )
	)
)

; pruneDomains: Takes two args, a list of unassigned variables of form 
; (row (domain)) and the current state of the board.
; Returns a list of unassigned variables and their legal domains in form
; (row (legal domain)).
(defun pruneDomains (unassigned n.queens)
	(cond ((null unassigned) '())
		  (t (let* ( (curVar (first (first unassigned)))
		  			 (curDom (second (first unassigned)))
		  			 (pruned (tryVar n.queens curDom curVar))
		  		   )
		  		(append (list (list curVar pruned)) (pruneDomains (rest unassigned) n.queens))
		     )
		  )
		)
)

; computeMinVar: Takes two arguments, a list of unassigned variables of form
; (row (domain)) and a minimum value to check against.
; Returns one minimum variable and its legal domain in form (var (domain)).
(defun computeMinVar (unassigned min)
	(cond ((null unassigned) (list -1 -1)) ; (domainsize rownum)
		  (t (let* ( (curVar (first (first unassigned)))
		  			 (curDom (second (first unassigned)))
		  			 (curDomSize (length curDom))
		  			 (minVar (if (< curDomSize min) ; Compute minimum variable, domain size pair.
		  			 				 (computeMinVar (rest unassigned) curDomSize)
		  			 				 (computeMinVar (rest unassigned) min)
		  			 			 ))
		  			 (minVar (if (< (first minVar) 0)
		  			 			 (list curVar curDom)
		  			 			  minVar
		  			 	))
		  			)
					  (if (< (length (second minVar)) curDomSize)
					  	   minVar
					  	  (list curVar curDom)
					  )
		  	))
		)
)

; MRV high level overview: Given a list and a domain of remaining values, check
; each variable in the list that is unassigned and try to assign it a value from
; domain D. Count the number of assignments that are successful.
; MRV: Takes two args, the current state of the board and list of unassigned
; columns.
; Returns most restricted variable and its legal domain in format (var (domain))
; MRV calls findUnassigned( n.queens domain ) that returns list of (row# (domain))
; findUnassigned calls firstUnassigned( n.queens ) that returns single row 
; number of first unassigned var.
; MRV calls pruneDomains(unassigned, n.queens), which returns (row# (legal domain))
; pruneDomains calls tryVar( n.queens domain row ) to determine legal domain.
(defun MRV( n.queens domain )
	(cond ((equal (count nil n.queens)
				  (length n.queens)) (if (oddp (length n.queens))
				  						 (list (/ (- (length n.queens) 1) 2) domain)
				  						 (list (/ (length n.queens) 2) domain)
				  					 ))
		  (t ( let*( (unassigned (findUnassigned n.queens domain 0))
		  			 (unassigned (pruneDomains unassigned n.queens))
					 (minVar (computeMinVar unassigned (length domain)))
		  		   )
		  		minVar
		  	  )
		  )
	)
)
; makeAssignment: Takes three args, the state of the board, the current domain,
; and the variable to assign values to.
; Returns set of assignments to backtrack on.
(defun makeAssignment(n.queens domain row)
	(cond ((null domain) '())
		  (t (let*( (checkState (append (butlast n.queens (- (length n.queens) (- row 1))) 
		  								(append (list (first domain)) (nthcdr row n.queens))))
		  		  )
		  		  (if (not (isLegal checkState))
		  		  	  (makeAssignment n.queens (rest domain) row)
		  		  	  (append (list checkState) (makeAssignment n.queens (rest domain) row))
		  		  )
		  	  )
		  )
	)
)

; backtrackHelper: Takes two arguments, a set of partial states and the 
; remaining values to assign.
; Returns an assignment or nil from set of possible successor states.
(defun backtrackHelper( assignment domain )
	(cond ((null assignment) nil) ; No more nodes to expand.
		  (t (let* ( (p.sol (backtracker (first assignment) (computeDomain (first assignment) domain)))
		  		   )
		  		(if (null p.sol)
		  			(backtrackHelper (rest assignment) domain)
		  			p.sol
		  		)
		  	  )
		  ) 
	)
)

; backtrackHelper: Takes two arguments, the current state of the board and the 
; remaining values to assign.
; Returns the goal state or nil.
(defun backtracker( n.queens domain )
	(cond ((isGoal n.queens) n.queens)
		  (t (let*( (m.var (MRV n.queens domain))
		  			(m.row (first m.var))
		  			(m.dom (second m.var))
		  			(m.assign (makeAssignment n.queens m.dom m.row))
		  			(n.assign (backtrackHelper m.assign domain))
		  	   	  )
			  	 (if (null n.assign)
			  	 	 	   nil
			  	 	 	   n.assign
			  	 )
		  	)
		)
	)
)

; QUEENS: Takes one argument, the number of queens (N) and attempts to find
; a solution to the N-queens problem.
(defun QUEENS(N)
	(cond ((equal N 0) '())
		  ((equal N 1) (list 1))
		  ((equal N 2) '())
		  ((equal N 3) '())
		  (t (let*( (domain (initDomain 1 N))
		  			(n.queens (initAssign N))
		  		  )
		  	  (backtracker n.queens domain)
			 )
		  )
	)
)

