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
env.Replace(CC='avr-g++')
ccflags = [
    "-O"+GetOption('opt'),
    "-DF_CPU={0:0.0f}UL".format(GetOption('freq')),
    "-mmcu=atmega328p",
    "-c",
    ]
env.Append(CCFLAGS=ccflags)
env.Append(LINKFLAGS=['-mmcu=atmega328p'])

#############
# Libraries #
#############
libs, incs = SConscript('libs/SConscript', exports=['env'])
for name in ['arduino', 'SPI', 'Wire', 'SoftwareSerial']:
    libfname = 'lib{0}.a'.format(name)
    libs[name] = File(os.path.join(ARDUINO_LIB, libfname))
    incs[name] = Dir(ARDUINO_INC)

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
size = env.Command('size', apps.values(), "avr-size $SOURCE")

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
