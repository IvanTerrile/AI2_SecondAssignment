(	
	define (domain localization)

	(:requirements :strips  :typing :durative-actions :numeric-fluents :negative-preconditions :action-costs :conditional-effects :equality :fluents)

	(:types 	
		robot 
		region 
		assignment
	)

	(:predicates
			(robot_in ?v - robot ?r - region) 
			(visited ?r - region )
			(taken ?a - assignment ?r - region)
			(assignment_at ?a - assignment ?r - region)  
	)

	(:functions 
			(act-cost) (triggered ?from ?to - region) (dummy)
	)

	(:durative-action goto_region_and_take_assignment
			:parameters (?v - robot ?from ?to - region ?a - assignment)
			:duration (= ?duration 100)
			:condition (and (at start (robot_in ?v ?from)))
				:effect (and (at start (not (robot_in ?v ?from))) (at start (increase (triggered ?from ?to) 1))
			(at end (robot_in ?v ?to)) (at end (assign (triggered ?from ?to) 0)) (at end (visited ?to)) (at end (taken ?a ?to))	
					(at end (increase (act-cost) (dummy))))
	)

	(:durative-action goto_region_and_submit_assignment
			:parameters (?v - robot ?from ?to - region ?a - assignment)
			:duration (= ?duration 100)
			:condition (and (at start (robot_in ?v ?from)) (at start (taken ?a ?from)))
				:effect (and (at start (not (robot_in ?v ?from))) (at start (increase (triggered ?from ?to) 1))
			(at end (robot_in ?v ?to)) (at end (assign (triggered ?from ?to) 0)) (at end (visited ?to)) (at end (assignment_at ?a ?to))	
					(at end (increase (act-cost) (dummy))))
	)
)