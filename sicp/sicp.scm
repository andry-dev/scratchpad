

(define (average-damp func)
  (lambda (x) (average x (func x))))

