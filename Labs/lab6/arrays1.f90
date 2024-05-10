!----------------------------------------------------------------------------- 
! Program code for CS 415/515 Parallel Programming, Portland State University.
!----------------------------------------------------------------------------- 

program arrays1

    parameter(n=8)
    integer, dimension(1:n) :: A, Reverse, Shuffle
    integer, dimension(1:n/2) :: Even, Odd, Front, Back
  
    A = (/ (i,i=1,n) /)       ! array initialization
    
    Odd = A(1:n:2)
    Even = A(2:n:2)

    Front = A(1:n/2)
    Back = A(n/2+1:n)

    Reverse = A(n:1:-1)

    forall (i = 1:n/2)
        Shuffle(2*i-1) = Front(i)
        Shuffle(2*i) = Back(i)
    end forall

    write(*, '(A,8I3)') 'A =', A
    write(*, '(A,4I3)') 'Odd =', Odd
    write(*, '(A,4I3)') 'Even =', Even
    write(*, '(A,4I3)') 'Front =', Front
    write(*, '(A,4I3)') 'Back =', Back
    write(*, '(A,8I3)') 'Reverse =', Reverse
    write(*, '(A,8I3)') 'Shuffle =', Shuffle

end program arrays1