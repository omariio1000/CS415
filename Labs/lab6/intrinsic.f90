!----------------------------------------------------------------------------- 
! Program code for CS 415/515 Parallel Programming, Portland State University.
!----------------------------------------------------------------------------- 

! Intrinsic functions

program intrinsic

  parameter (n=5)
  integer, dimension(n) :: a = (/1,3,5,7,9/)
  integer total, max, cnt
  
  total = sum(a)         ! = 25
  max = maxval(a)        ! = 9
  cnt = count(a>3)       ! = 3

  write(*, '(A,5I3)') 'a =', a
  write(*, '(A,3I3)') 'total, max, cnt =', total, max, cnt

end program intrinsic
