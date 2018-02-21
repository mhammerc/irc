import nclib
import unittest

class IRCMessage():
	def __init__(self, message, tests):
		self.prefix = None
		self.command = None
		self.dest = None
		self.params = None
		
		message = message.decode('ascii')
		tests.assertGreaterEqual(len(message), 2)
		tests.assertEqual(message[-2:], '\r\n')
		message = message[:-2]
		
		message = message.split(' ')
		self.prefix = message[0]
		self.command = message[1]
		self.dest = message[2]
		self.params = message[3:]

	def __str__(self):
		return ('IRCMessage prefix: ' + self.prefix
		+ ', command: ' + self.command
		+ ', dest: ' + self.dest
		+ ', params[0]: ' + self.params[0])

class TestIRCMethods(unittest.TestCase): 

	def __init__(self, x):
		super().__init__(x)
		self.s = None
		self.username = 'mhammerc'

	def setUp(self):
		self.s = nclib.Netcat(connect=('127.0.0.1', 3000), verbose=False)
	
	def tearDown(self):
		self.s.close()

	def send(self, message):
		self.s.send(message.encode('ascii') + b'\r\n')

	def recv(self, verbose = None):
		message = self.s.recv_until(b'\r\n')

		if verbose:
			print(message)

		message = IRCMessage(message, self)
		self.assertDest(message)
		return message

	def assertDest(self, message):
		self.assertEqual(message.dest, self.username)
	
	def assertCommand(self, message, command):
		self.assertEqual(message.command, command)

	def test_identifying(self):
		self.send('NICK ' + self.username)
		self.send('USER ' + self.username + ' 0 * :Martin Hammerchmidt')
		
		welcome_1 = self.recv()
		welcome_2 = self.recv()
		welcome_3 = self.recv()
		welcome_4 = self.recv()

		self.assertEqual(welcome_1.command, '001')
		self.assertEqual(welcome_2.command, '002')
		self.assertEqual(welcome_3.command, '003')
		self.assertEqual(welcome_4.command, '004')

	def test_mode1(self):
		self.send('NICK ' + self.username)
		self.send('USER ' + self.username + ' 0 * :Martin Hammerchmidt')
		
		# pass the 4 greeting messages
		self.recv()
		self.recv()
		self.recv()
		self.recv()

		self.send('MODE mhammerc')
		mode = self.recv()
		self.assertCommand(mode, '221')
		self.assertEqual(mode.params[0], '+')

		self.send('MODE mhammerc +i')
		mode = self.recv()
		self.assertCommand(mode, '221')
		self.assertIn('+', mode.params[0])
		self.assertIn('i', mode.params[0])
		
		self.send('MODE mhammerc +w')
		mode = self.recv()
		self.assertCommand(mode, '221')
		self.assertIn('+', mode.params[0])
		self.assertIn('i', mode.params[0])
		self.assertIn('w', mode.params[0])

		self.send('MODE mhammerc -i+r')
		mode = self.recv()
		self.assertCommand(mode, '221')
		self.assertIn('+', mode.params[0])
		self.assertIn('w', mode.params[0])
		self.assertIn('r', mode.params[0])
		self.assertNotIn('i', mode.params[0])

		self.send('MODE mhammerc')
		mode = self.recv()
		self.assertCommand(mode, '221')
		self.assertIn('+', mode.params[0])
		self.assertIn('w', mode.params[0])
		self.assertIn('r', mode.params[0])
		self.assertNotIn('i', mode.params[0])
	
	def test_mode2(self):
		self.send('NICK ' + self.username)
		self.send('USER ' + self.username + ' 4 * :Martin Hammerchmidt')
		
		# pass the 4 greeting messages
		self.recv()
		self.recv()
		self.recv()
		self.recv()

		self.send('MODE mhammerc')
		mode = self.recv()
		self.assertCommand(mode, '221')
		self.assertIn('+', mode.params[0])
		self.assertIn('w', mode.params[0])
		self.assertNotIn('i', mode.params[0])
		self.assertNotIn('r', mode.params[0])
		self.assertNotIn('a', mode.params[0])
		self.assertNotIn('o', mode.params[0])
		self.assertNotIn('O', mode.params[0])
	
	def test_mode3(self):
		self.send('NICK ' + self.username)
		self.send('USER ' + self.username + ' 8 * :Martin Hammerchmidt')
		
		# pass the 4 greeting messages
		self.recv()
		self.recv()
		self.recv()
		self.recv()

		self.send('MODE mhammerc')
		mode = self.recv()
		self.assertCommand(mode, '221')
		self.assertIn('+', mode.params[0])
		self.assertIn('i', mode.params[0])
		self.assertNotIn('w', mode.params[0])
		self.assertNotIn('r', mode.params[0])
		self.assertNotIn('a', mode.params[0])
		self.assertNotIn('o', mode.params[0])
		self.assertNotIn('O', mode.params[0])

	def test_mode4(self):
		self.send('NICK ' + self.username)
		self.send('USER ' + self.username + ' 12 * :Martin Hammerchmidt')
		
		# pass the 4 greeting messages
		self.recv()
		self.recv()
		self.recv()
		self.recv()

		self.send('MODE mhammerc')
		mode = self.recv()
		self.assertCommand(mode, '221')
		self.assertIn('+', mode.params[0])
		self.assertIn('i', mode.params[0])
		self.assertIn('w', mode.params[0])
		self.assertNotIn('r', mode.params[0])
		self.assertNotIn('a', mode.params[0])
		self.assertNotIn('o', mode.params[0])
		self.assertNotIn('O', mode.params[0])

	def test_mode5(self):
		self.send('NICK ' + self.username)
		self.send('USER ' + self.username + ' 0 * :Martin Hammerchmidt')
		
		# pass the 4 greeting messages
		self.recv()
		self.recv()
		self.recv()
		self.recv()

		self.send('MODE')
		mode = self.recv()
		self.assertCommand(mode, '461')

		self.send('MODE notMhammerc')
		mode = self.recv()
		self.assertCommand(mode, '502')

		# auto-opping via MODE is ignored. We must use OPER command.
		# But it doesn't generate an error.
		self.send('MODE mhammerc +o')
		mode = self.recv()
		self.assertCommand(mode, '221')
		self.assertNotIn('o', mode.params[0])

		self.send('MODE mhammerc +O')
		mode = self.recv()
		self.assertCommand(mode, '221')
		self.assertNotIn('O', mode.params[0])

	def test_mode6(self):
		self.send('NICK ' + self.username)
		self.send('USER ' + self.username + ' 0 * :Martin Hammerchmidt')
		
		# pass the 4 greeting messages
		self.recv()
		self.recv()
		self.recv()
		self.recv()

		self.send('MODE mhammerc +U')
		mode = self.recv()
		self.assertCommand(mode, '501')

		self.send('MODE mhammerc')
		mode = self.recv()
		self.assertCommand(mode, '221')
		self.assertIn('+', mode.params[0])
		self.assertNotIn('i', mode.params[0])
		self.assertNotIn('w', mode.params[0])
		self.assertNotIn('r', mode.params[0])
		self.assertNotIn('a', mode.params[0])
		self.assertNotIn('o', mode.params[0])
		self.assertNotIn('O', mode.params[0])


if __name__ == '__main__':
	unittest.main()