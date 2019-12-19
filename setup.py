from setuptools import setup, Extension, find_packages



wrapper = Extension('socketftp',
                    define_macros = [('MAJOR_VERSION', '0'),
                                     ('MINOR_VERSION', '1')],
                    libraries =['socketftp'],
                    sources =['wrapper.c']
                    )

setup(name='pysocketftp',
      version='0.1',
      description='server client socket',
      author='Kalkidan Fekadu',
      author_email='fkalkidan30@gmail.com',
      license='MIT',
      ext_modules = [wrapper],
      zip_safe=False)


