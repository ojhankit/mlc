'''from setuptools import setup, Extension

module = Extension('primes', sources=['primesmodule.c'])

setup(
    name='primes',
    version='1.0',
    ext_modules=[module],
)'''

from setuptools import setup, Extension

mini_ml_module = Extension(
    name='mini_ml',                 
    sources=[
        'csrc/vector.c',            # Vector functions
        'csrc/matrix.c',            # Matrix functions
        'csrc/wrapper.c',           # Python wrapper functions
    ],
    include_dirs=['csrc'],
    extra_compile_args=['-O2', '-Wall', '-fPIC'],  # Optimizations & warnings
)

# Setup script
setup(
    name='mini_ml',
    version='1.0',
    description='Mini ML library in C for Python',
    ext_modules=[mini_ml_module],
)

