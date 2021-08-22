# Linear algebra
## Point
* A _point_ represents a position within a coordinate system.
* Each of the numbers in the point's sequence is called a _coordinate_.
* The number of coordinates is the point's _dimension_.

## Vector
* A _vector_ represents the difference between two points.
* Despite sharing their representation with points, vectors don't represent or have a position.
* Vectors are characterized by their _direction_ (the angle in which they point) and their _magnitude_ (how long they are).
* The direction can be further decomposed into _orientation_ (the slope of the line they're on) and _sense_ (which of the possible two ways along that line they point).

### Vector magnitude
* The magnitude is also called the _length_ or _norm_ of the vector.
* `|V| = sqrt(V_x^2 + V_y^2 + V_z^2)`.
* `|V| = sqrt([V,V]) (see Dot product)`.
* A vector with a magnitude equal to 1.0 is called a _unit vector_.

## Point and vector operations
### Subtracting points
* A vector is the difference between two points.
* `Point_finish - Point_start = Vector`.

### Adding a point and a vector
* You can add a vector to a point and get a new point.
* This makes intuitive and geometric sense; given a starting position (a point) and a displacement (a vector), you end up in a new position (another point).
* `Point + Vector = Point`.

### Adding vectors
* You can add two vectors.
* Geometrically, imagine putting one vector "after" another.
* `Vector + Vector = Vector`.

### Multiplying a vector by a number
* This is called the _scalar product_.
* This makes the vector shorter or longer.
* `Scalar * Vector = Vector`.
* One of the applications of vector multiplication and division is to _normalize_ a vector -- that is, to turn it into a unit vector.
* `V_normalized = V / |V|`.

### Multiplying vectors
#### Dot product
* The _dot product_ between two vectors (also called the _inner product_) gives you a number.
* `[V,W] = V_x*W_x + V_y*W_y + V_z*W_z`.
* Geometrically, the dot product of ⃗V and W is related to their lengths and to the angle alpha between them.
* `[V,W] = |V| * |W| * cos(alpha)`.
* `[V,V] = |V|^2`.

#### Cross product
* The _cross product_ of two vectors is a vector perpendicular to both of them.
* `R = V x W = (V_y*W_z - V_z*W_y, V_x*W_z - V_z*W_x, V_X*W_y - V_y*W_x)`.
* The cross product is not commutative. Specifically, `V x W ⃗ = –(W x V)`.

## Matrices
* A _matrix_ is a rectangular array of numbers.
* Matrices represent _transformations_ that can be applied to points or vectors.
* A matrix is characterized by its size in terms of columns and rows.

### Matrix operations
#### Adding matrices
* You can add two matrices, as long as they have the same size.
```
| a b c |   | j k l |   | a+j b+k c+l |
| d e f | + | m n o | = | d+m e+n f+o |
| g h i |   | p q r |   | g+p h+q i+r |
```

#### Multiplying matrix by a number
* You can multiply a matrix by a number.
```
    | a b c |   | n*a n*b n*c |
n * | d e f | = | n*d n*e n*f |
    | g h i |   | n*g n*h n*i |
```

#### Multiplying matrices
* You can multiply two matrices together, as long as their sizes are compatible: the number of columns in the first matrix must be the same as the number of rows in the second matrix.
* The result of multiplying two matrices together is another matrix, with the same number of rows as the left-hand side matrix, and the same number of columns as the right-hand side matrix.
* The value of the element of the result is the dot product of the corresponding row vector in A and column vector in B.
```
| - A_0 - |   |  |   |   |   |  |
| - A_1 - | * | B_0 B_1 B_2 B_3 | = | [A_0,B_0] [A_0,B_1] [A_0,B_2] [A_0,B_3] |
              |  |   |   |   |  |   | [A_1,B_0] [A_1,B_1] [A_1,B_2] [A_1,B_3] |
```

#### Multiplying a matrix and a vector
* You can think of an n-dimensional vector as either an n x 1 vertical matrix or as a 1 x n horizontal matrix, and multiply the same way you would multiply any two compatible matrices.
```
| a b c |   | x |
| d e f | * | y | = (a*x + b*y + c*z, d*x + e*y + f*z)
            | z |
```
* Since the result of multiplying a matrix and a vector (or a vector and a matrix) is also a vector and, in our case, matrices represent transformations, we can say that the matrix _transforms_ the vector.
