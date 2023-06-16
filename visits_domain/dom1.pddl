(define (domain localization)

(:requirements :typing :durative-actions :numeric-fluents :negative-preconditions :action-costs :conditional-effects :equality :fluents )


(:types 	
	robot 
	region 
	; assignment
)

(:predicates
		(robot_in ?v - robot ?r - region) 
		(visited ?r - region )
		; (taken ?a - assignment)
		; (submitted ?a - assignment)
		; (assignment_at ?a - assignment ?r - region)
	      
)

(:functions 
		(act-cost) (triggered ?from ?to - region) (dummy)
)

(:durative-action goto_region
		:parameters (?v - robot ?from ?to - region)
		:duration (= ?duration 100)
		:condition (and (at start (robot_in ?v ?from)))
	        :effect (and (at start (not (robot_in ?v ?from))) (at start (increase (triggered ?from ?to) 1))
		(at end (robot_in ?v ?to)) (at end (assign (triggered ?from ?to) 0)) (at end (visited ?to)) 	
                (at end (increase (act-cost) (dummy))))
)

; (:action collect_assignment
;         :parameters (?a - assignment ?r - region ?v - robot)
;         :precondition (and (assignment_at ?a ?r) (robot_in ?v ?r) (not (taken ?a)))
;         :effect (and (not (assignment_at ?a ?r)) (taken ?a))
; )

; (:action submit_assignment
;         :parameters (?a - assignment ?r - region ?v - robot)
;         :precondition (and (not (assignment_at ?a ?r)) (robot_in ?v ?r) (taken ?a) (not (submitted ?a)))
;         :effect (and (assignment_at ?a ?r) (not (taken ?a)) (submitted ?a))
; )


;;(:durative-action localize
;; ...................
;;)

)