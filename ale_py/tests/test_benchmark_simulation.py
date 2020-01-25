import os
import numpy as np

from ale_py import ALEInterface
from ale_py_pybind import ALEInterface as ALEBindInterface

ITERATIONS = 5000

def ale(pybind=False):
    root = os.path.dirname(__file__)
    rom = os.path.join(root, "fixtures", "tetris.bin")

    if pybind:
        _ale = ALEBindInterface()
    else:
        _ale = ALEInterface()
    _ale.loadROM(rom)
    _ale.setInt("random_seed", 0)

    return _ale


def simulate(_ale, steps, preallocate=True, pybind=False):
    _ale.reset_game()
    action_set = _ale.getLegalActionSet()

    if preallocate:
        img = np.zeros((210, 160, 3), dtype=np.uint8)

    for step in range(steps):
        action = np.random.randint(len(action_set))
        _ale.act(action_set[action])
        _ale.getScreenRGB(img if preallocate else None)
        if _ale.game_over():
            _ale.reset_game()

def test_sim_no_preallocate(benchmark):
    np.random.seed(0)
    _ale = ale()
    benchmark(simulate, _ale, ITERATIONS, preallocate=False)
    assert True


def test_sim_preallocate(benchmark):
    np.random.seed(0)
    _ale = ale()
    benchmark(simulate, _ale, ITERATIONS, preallocate=True)
    assert True


def test_pybind_preallocate(benchmark):
    np.random.seed(0)
    _ale = ale(pybind=True)
    benchmark(simulate, _ale, ITERATIONS, preallocate=True, pybind=True)

    assert True
