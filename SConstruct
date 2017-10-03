HexObject = Builder(
    action="$OBJCOPY -O ihex -R .eeprom $SOURCE $TARGET",
    suffix = '.hex', src_suffix = '.o'
)
builders = dict(HexObject=HexObject)
env = Environment(BUILDERS=builders)

#################
# Build options #
#################

AddOption(
    '--flash',
    default=False,
    dest='flash',
    action='store_true',
    metavar='FLASH',
    help='avrdude flash'
)


AddOption(
    '--opt',
    default='1',
    dest='opt',
    type='string',
    nargs=1,
    action='store',
    metavar='OPT',
    help='CC optimization level'
)

AddOption(
    '--port',
    default='/dev/ttyUSB0',
    dest='port',
    type='string',
    nargs=1,
    action='store',
    metavar='PORT',
    help='avrdude flash device port'
)

#####################
# Build environment #
#####################

env.Replace(CC='avr-gcc')
env.Replace(CXX='avr-g++')
env.Replace(AR='avr-ar')
env.Replace(OBJCOPY='avr-objcopy')

ccflags = [
    "-O"+GetOption('opt'),
    "-DF_CPU=16000000UL",
    "-DARDUINO=101",
    "-mmcu=atmega328p",
]
env.Append(CCFLAGS=ccflags)

cxxflags = ["-fno-use-cxa-atexit"]
env.Append(CXXFLAGS=cxxflags)

linkflags = [
    "-mmcu=atmega328p",
    "-ffunction-sections",
    "-fdata-sections",
]
env.Append(LINKFLAGS=linkflags)

env.SConscript('SConscript', exports=['env'])
