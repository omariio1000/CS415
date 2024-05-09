!----------------------------------------------------------------------------- 
! Program code for CS 415/515 Parallel Programming, Portland State University.
!----------------------------------------------------------------------------- 

! Forall loops

program forall

  parameter (N=5)
  integer, dimension(1:N) :: a, b, c

  a = (/ (i,i=1,N) /)       ! array initialization
  b = 1

  forall (i = N:1:-1)       ! descending range is OK
     c(i) = a(i)
  end forall  

  forall (i = 1:N-1)       
    a(i) = b(i) + c(i)     
    a(i+1) = a(i) + a(i+1) 
  end forall               

  write(*, '(A,5I4)') 'a: ', a
  write(*, '(A,5I4)') 'b: ', b
  write(*, '(A,5I4)') 'c: ', c

end program forall
