!----------------------------------------------------------------------------- 
! Program code for CS 415/515 Parallel Programming, Portland State University.
!----------------------------------------------------------------------------- 

program arrayop

  ! Array operations

  parameter (m=4, n=6)
  integer, dimension(1:n) :: a, b, c
  integer:: array(m,n), row1(n), row2(n), row3s(n/2), inner(m-2,n-2)

  a = (/ (i,i=1,n) /)       ! array initialization
  b = 2                     ! scalar expansion
  c = a**2 + b**2           ! whole array operations
  write(*, '(A,6I3)') 'a =', a
  write(*, '(A,6I3)') 'b =', b
  write(*, '(A,6I3)') 'c =', c

  ! Array section operations

  array = reshape((/(i,i=1,m*n)/),(/4,6/))
  row1 = array(1,1:n)       ! stride=1 can be omitted
  row2 = array(2,:)         ! both low and high can be omitted
  row3s = array(3,1:n:2)
  inner = array(2:m-1,2:n-1)

  write(*, '(A)') 'array:'
  write(*, '(6I3)') array
  write(*, '(A,6I3)') 'row1:', row1
  write(*, '(A,6I3)') 'row2:', row2
  write(*, '(A,6I3)') 'row3:', row3s
  write(*, '(A)') 'inner:'
  write(*, '(4I3)') inner

end program arrayop
