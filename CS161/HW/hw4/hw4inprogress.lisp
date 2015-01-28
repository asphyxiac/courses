;; Takes one integer argument, x, and returns the absolute value of x.
( defun absX (x) 
	(cond ((< x 0) (* x -1)) ; If x < 0, multiply it by -1.
		  (t x) ))


; given a state, n.queens, need to check if any two n.queens are in same diagonal.
; Need to do a pairwise check of n.queens.
; Function to recursively check n.queens:
; first call: r1 - r2 = 1, c1 - c2 = first n.queens - first (rest n.queens)
; check if r1 - r2 = abs(c1 - c2)

(defun checkDiag (n.queens)
	(cond ((null n.queens) t)
		  ((null (first n.queens)) (checkDiag (rest n.queens)))
		  ((null (rest n.queens)) t)
		  (t (and (checkPairwiseDiag 1 (first n.queens) (rest n.queens)) (checkDiag (rest n.queens))))
	)
)

(defun checkPairwiseDiag (delta-r col rest-n.queens) 
	(cond ((null rest-n.queens) t)
		  ((null (first rest-n.queens)) (checkPairwiseDiag (+ delta-r 1) col (rest rest-n.queens)))
		  (t (and (not (equal delta-r (absX (- col (first rest-n.queens)))))
		  	      (checkPairwiseDiag (+ delta-r 1) col (rest rest-n.queens))))
		  ))

; Need to make sure that all n.queens are in different rows/cols:
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

; Need to find a way to select unassigned variable (row).
; Choose the most constrained variable, i.e., the row with
; the fewest remaining values.
; Given a list and a domain of remaining values, check
; each variable in the list that is unassigned and try to assign
; it a value from domain D. Count the number of assignments
; that are successful.
; Need to find first unassigned variable.
;		If first n.queens == nil, return 1.
;		If first n.queens != nil, return 1 + recursion on (rest n.queens)
; Need to find rest of the unassigned variables:
; 		We know the row of the first unassigned variable.
;		Find the row of the unassigned variables in the remaining list.
;		n.queens <- (1 2 nil nil nil 3 nil ) row of first unassigned variable is 3.
;		nthcdr 3 n.queens returns (nil nil 3 nil)
;		butlast n.queens 4 returns (1 2 nil)
;		row of next unassigned variable is row+1
; Need to try assigning first unassigned variable.
;		If domain(var) is empty, return empty list.
;		Assign var in row first value from domain(var)
;		If assignment is not legal, domain(var) = rest domain(var) (recursive call)
;		If assignment is legal, domain(var) = append (list first domain(var)) tryVar( row (rest domain(var)))
; Need to compute number of successful assignments.
; 		Represent unassigned variables as (row#, domain(var)).
;		get row# from findNil, assume domain(var) = domain(all).
;		Domain size = length(domain(var)).
; Need to compute minimum domain size and return appropriate row.
; 		Find smallest domain size.


; MRV calls findUnassigned( n.queens domain ) that returns list of (row#, domain).
; findUnassigned calls firstUnassigned( n.queens ) that returns single row number of first unassigned var.
; MRV calls pruneDomains(unassigned, n.queens), which returns (row#, legal domain)
; pruneDomains calls tryVar( n.queens domain row )
; unassigned <- findUnassigned( n.queens domain 0 )
; unassigned <- pruneDomains( unassigned n.queens )
; compute minimum domain size in unassigned, return minimum value.
(defun isGoal (n.queens) 
	(cond ((equal (count nil n.queens) 0) t)
		  (t nil)
	)
)

(defun initAssign(N)
	(cond ((<= N 0) '())
		  (t (append (list nil) (initAssign (- N 1))))
	)
)

(defun initDomain(start N)
	(cond ((> start N) '())
          (t (cons start (initDomain (+ start 1) N)))
    )
 )

(defun computeDomain (n.queens domain)
	(cond ((null domain) '())
		  ((> (count (first domain) n.queens) 0) (computeDomain n.queens (rest domain)))
		  (t (append (list (first domain)) (computeDomain n.queens (rest domain))))
	)
)

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

; Return an assignment
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

(defun firstUnassigned( n.queens )
	(cond ((null (first n.queens)) 1)
		  (t (+ 1 (firstUnassigned (rest n.queens))))
	)
)
; Base case: n.queens is null, return '().
; Else, get first unassigned row from firstUnassigned(n.queens)
(defun findUnassigned ( n.queens domain start )
	(cond ((null n.queens) '())
		  ((> start (length domain)) '())
		  (t (let* ( (row (+ start (firstUnassigned (nthcdr start n.queens))))
		  		   )
		  		(append (list (list row domain)) (findUnassigned n.queens domain row))
		  	 )
		  ))
)

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


(defun isLegal(n.queens) 
	(cond ((and (checkDiag n.queens) (allDifferent n.queens)) t)
		  (t nil)
	)
)




