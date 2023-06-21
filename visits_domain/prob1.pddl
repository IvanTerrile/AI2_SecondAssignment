(define (problem prob1)

     (:domain localization)

     (:objects
          r0 r1 r2 r3 r4 r5 - region
          R2D2 - robot
          a1 a2 a3 a4 - assignment
     )

     (:init
          (robot_in R2D2 r0)
          (assignment_at a1 r1)
          (assignment_at a2 r2)
          (assignment_at a3 r3)
          (assignment_at a4 r4)

          (= (act-cost) 0)
          (= (dummy) 0)
     )

     (:goal 
          (and 
          (visited r1) (taken a1 r1) (taken a2 r2)
          (assignment_at a1 r5) (visited r2)
               (assignment_at a2 r5)
          )
     )

     (:metric minimize (act-cost) )
)


