#ifndef _OPENMP
FUNCTION omp_get_num_teams() RESULT(i)
    INTEGER :: i
    i = 1
END FUNCTION omp_get_num_teams
FUNCTION omp_get_num_threads() RESULT(i)
    INTEGER :: i
    i = 1
END FUNCTION omp_get_num_threads
#endif
FUNCTION almost_equal(x, gold, tol) RESULT(b)
    implicit none
    DOUBLE COMPLEX, intent(in) :: x
    INTEGER,  intent(in) :: gold
    REAL,     intent(in) :: tol
    LOGICAL              :: b
    b = ( gold * (1 - tol)  <= ABS(x) ).AND.( ABS(x) <= gold * (1+tol) )
END FUNCTION almost_equal
PROGRAM target__teams_distribute__parallel
#ifdef _OPENMP
    USE OMP_LIB
    implicit none
#else
    implicit none
    INTEGER :: omp_get_num_threads
#endif
    LOGICAL :: almost_equal
    INTEGER :: N_i = 64
    INTEGER :: i
    DOUBLE COMPLEX :: counter = (0,0)
    INTEGER :: num_threads
!$OMP TARGET MAP(TOFROM: counter) 
!$OMP TEAMS DISTRIBUTE REDUCTION(+: counter)
    DO i = 1 , N_i
!$OMP PARALLEL REDUCTION(+: counter)
    num_threads = omp_get_num_threads()
counter = counter +  CMPLX(  1./num_threads , 0 ) 
!$OMP END PARALLEL
    END DO
!$OMP END TARGET
IF ( .NOT.almost_equal(counter, N_i, 0.1) ) THEN
    WRITE(*,*)  'Expected', N_i,  'Got', counter
    CALL EXIT(112)
ENDIF
END PROGRAM target__teams_distribute__parallel