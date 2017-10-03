Import('env')

#############
# Libraries #
#############

libs = SConscript('libs/SConscript', exports=['env'])

################
# Applications #
################

apps = SConscript('apps/SConscript', exports=['env', 'libs'])

###########
# Aliases #
###########

for name, hex_ in apps.items():
    env.Alias(name, hex_)

#size = env.Command('size', apps.values(), "avr-size $SOURCES")
#selected = list(set(COMMAND_LINE_TARGETS).intersection(apps))
#if not selected:
#    Default(None)
#elif GetOption('flash'):
#    app = apps[selected[0]]
#    avrdude = ' '.join([
#        'sudo avrdude',
#        '-F',
#        '-V',
#        '-e',
#        '-c arduino',
#        '-p ATMEGA328P',
#        '-P'+GetOption('port'),
#        '-b 115200',
#        '-U flash:w:$SOURCE:i',
#        ])
#    flash = env.Command('flash', app, avrdude)
#    Default(flash)

clean_targets = [libs.values(), apps.values()]
defaults = clean_targets if GetOption('clean') else []
env.Default(*defaults)
