#################
# Build options #
#################

AddOption(
    '--flash',
    default=False,
    dest='flash',
    action='store_true',
    help='avrdude flash'
)

AddOption(
    '--size',
    default=False,
    dest='size',
    action='store_true',
    help='flash rom size'
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


############
# Builders #
############

HexObject = Builder(
    action="$OBJCOPY -O ihex -R .eeprom $SOURCE $TARGET",
    suffix = '.hex', src_suffix = '.elf'
)

avrdude = ' '.join([
    '$AVRDUDE',
    '-F', '-V', '-e',
    '-c arduino',
    '-p ATMEGA328P',
    '-P'+GetOption('port'),
    '-b 115200',
    '-U flash:w:$SOURCE:i',
])
Flash = Builder(action=avrdude, suffix='.flash', src_suffix='.hex')

HexSize = Builder(
    action="$AVRSIZE $SOURCES",
    suffix = '.size', src_suffix = '.hex'
)

builders = dict(HexObject=HexObject, Flash=Flash, HexSize=HexSize)
env = Environment(BUILDERS=builders)

#####################
# Build environment #
#####################

env.Replace(CC='avr-gcc')
env.Replace(CXX='avr-g++')
env.Replace(AR='avr-ar')
env.Replace(OBJCOPY='avr-objcopy')
env.Replace(AVRDUDE='avrdude')
env.Replace(AVRSIZE='avr-size')

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

SConscript('SConscript', exports=['env'])
