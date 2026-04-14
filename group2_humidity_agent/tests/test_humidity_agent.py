import unittest
import numpy as np

from group2_humidity_agent.env.state_mapping import get_humidity_state
from group2_humidity_agent.env.reward import get_reward
from group2_humidity_agent.model.q_learning_humidity import Q, choose_action, update_q


class TestHumidityAgent(unittest.TestCase):

    def test_get_humidity_state(self):
        self.assertEqual(get_humidity_state(20), 0)
        self.assertEqual(get_humidity_state(55), 1)
        self.assertEqual(get_humidity_state(85), 2)

    def test_get_reward(self):
        self.assertEqual(get_reward(50), 10)
        self.assertEqual(get_reward(25), -10)
        self.assertEqual(get_reward(75), -2)

    def test_update_q_changes_table(self):
        Q.fill(0.0)
        prev = Q.copy()
        update_q(1, 2, 5, 0)
        self.assertFalse(np.allclose(Q, prev))

    def test_choose_action_return_type(self):
        action = choose_action(1)
        self.assertIn(action, {0, 1, 2})


if __name__ == '__main__':
    unittest.main()
