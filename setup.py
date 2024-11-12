# setup.py

from setuptools import setup, Extension

module = Extension('_behave',
                   sources=['behave.i'],
                   include_dirs=["src/behave"])

setup(name='example',
      version='1.0',
      ext_modules=[module])
