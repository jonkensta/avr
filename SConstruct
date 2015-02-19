import os

#################
# Build options #
#################

ARDUINO_INC = "/usr/share/arduino/include"
ARDUINO_LIB = "/usr/share/arduino/lib"

AddOption(
    '--opt',
    default='2',
    dest='opt',
    type='string',
    nargs=1,
    action='store',
    metavar='OPT',
    help='CC optimization level'
    )

AddOption(
    '--freq',
    default=16e6,
    dest='freq',
    type='float',
    nargs=1,
    action='store',
    metavar='CPU',
    help='MCU CPU clock frequency'
    )

AddOption(
    '--port',
    default='/dev/ttyACM0',
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
env = Environment()

env.Replace(CC='avr-gcc')
env.Replace(CXX='avr-g++')
env.Replace(AR='avr-ar')

ccflags = [
    "-O"+GetOption('opt'),
    "-DF_CPU={0:0.0f}UL".format(GetOption('freq')),
    "-mmcu=atmega328p",
    ]
env.Append(CCFLAGS=ccflags)
env.Append(CXXFLAGS=ccflags)

linkflags = [
    '-mmcu=atmega328p',
    '-gc-sections', 
    '-ffunction-sections',
    '-fdata-sections',
    ]
env.Append(LINKFLAGS=linkflags)

#############
# Libraries #
#############
libs, incs = SConscript('libs/SConscript', exports=['env'])

################
# Applications #
################
apps = SConscript('apps/SConscript', exports=['env', 'libs', 'incs'])
Default(apps.values())

for name, hex_ in apps.items():
    env.Alias(name, hex_)

all_ = env.Alias('all', apps.values())
Default(all_)

###########
# Aliases #
###########
size = env.Command('size', apps.values(), "avr-size $SOURCES")

selected = list(set(COMMAND_LINE_TARGETS).intersection(apps))
if selected:
    app = apps[selected[0]]
    avrdude = ' '.join([
        'sudo avrdude',
        '-F',
        '-V',
        '-e',
        '-c arduino',
        '-p ATMEGA328P',
        '-P'+GetOption('port'),
        '-b 115200',
        '-U flash:w:$SOURCE:i',
        ])
    flash = env.Command('flash', app, avrdude)
