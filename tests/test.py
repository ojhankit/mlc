# import primes

# # Check a number
# print(primes.is_prime(97))  # True

# # Generate primes up to 50
# print(primes.generate_primes(50))

import mini_ml
# Create a vector from a Python list
vec = mini_ml.py_create_vec([1.0, 2.0, 3.5, 4])

# Print the vector (C-side function)
mini_ml.py_print_vec(vec)

# Free memory (important!)
mini_ml.py_free_vec(vec)

# Create a 2x3 matrix
mat = mini_ml.py_create_mat([[1, 2, 3], [4, 5, 6]])

# Print the matrix (C-side function)
mini_ml.py_print_mat(mat)

# Free memory
mini_ml.py_free_mat(mat)
