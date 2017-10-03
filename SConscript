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

if GetOption('flash'):
    for name, hex_ in apps.items():
        env.Alias(name, env.Flash(hex_))
elif GetOption('size'):
    for name, hex_ in apps.items():
        env.Alias(name, env.HexSize(hex_))
else:
    for name, hex_ in apps.items():
        env.Alias(name, hex_)
    env.Alias('all', apps.values())

selected = list(set(COMMAND_LINE_TARGETS).intersection(apps))

if GetOption('clean'):
    clean_targets = [libs.values(), apps.values()]
    env.Default(clean_targets)

else:
    env.Default(None)
