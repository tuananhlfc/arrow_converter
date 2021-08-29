from conans import ConanFile, CMake

class ConanPackage(ConanFile):
    name = 'arrow_example'
    version = '0.1.0'
    generators = 'cmake_find_package'

    requires = [
        ('arrow/2.0.0')
    ]

    default_options = (
        'arrow:shared=True',
    )
