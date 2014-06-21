#################
# Build options #
#################
AddOption(
    '--mcu',
    default='atmega328p',
    dest='mcu',
    type='string',
    nargs=1,
    action='store',
    metavar='MCU',
    help='microcontroller unit'
    )

AddOption(
    '--opt',
    default='s',
    dest='opt',
    type='string',
    nargs=1,
    action='store',
    metavar='OPT',
    help='CC optimization level'
    )

AddOption(
    '--cpu',
    default=16e6,
    dest='cpu',
    type='float',
    nargs=1,
    action='store',
    metavar='CPU',
    help='MCU CPU clock frequency'
    )

AddOption(
    '--programmer',
    default='arduino',
    dest='prog',
    type='string',
    nargs=1,
    action='store',
    metavar='PROG',
    help='avrdude programmer type'
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
ccflags = [
    "-O"+GetOption('opt'),
    "-DF_CPU={0:0.0f}UL".format(GetOption('cpu')),
    "-mmcu="+GetOption('mcu'),
    "-c",
    ]
env.Append(CCFLAGS=ccflags)
env.Replace(CC='avr-gcc')


################
# Applications #
################
apps = SConscript('apps/SConscript', exports=['env']) # Dict of apps

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
        '-c'+GetOption('prog'),
        '-p'+GetOption('mcu').upper(),
        '-P'+GetOption('port'),
        '-b 115200',
        '-U flash:w:$SOURCE:i',
        ])
    flash = env.Command('flash', app, avrdude)
