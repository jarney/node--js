def pytest_addoption(parser):
    parser.addoption('--data-directory',
                     default='q',
                     help='Location of test data files',
                     action='store')

#print("Reading option parser?")
#exit(1)
