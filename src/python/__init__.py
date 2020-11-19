import platform
import sys
import os

if (
    platform.system() == "Windows"
    and sys.version_info.major == 3
    and sys.version_info.minor >= 8
):
    packagedir = os.path.abspath(os.path.dirname(__file__))
    os.add_dll_directory(packagedir)


from ._ale_py import *
from .gym import register_environments

__version__ = '0.6.2'

register_environments()
