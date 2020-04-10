#!/usr/bin/env bash

REDIS_DIR=/var/run/redis$NS_ID
mkdir -p $REDIS_DIR/sonic-db

if [ -f /etc/sonic/database_config$NS_ID.json ]; then
    cp /etc/sonic/database_config$NS_ID.json $REDIS_DIR/sonic-db/database_config.json
else
    python -c "import sys;import jinja2;loader = jinja2.FileSystemLoader('/usr/share/sonic/templates/');env = jinja2.Environment(loader=loader, trim_blocks=True);template = env.get_template('database_config.json.j2');print(template.render(NS=sys.argv[1]))" "$NS_ID" > $REDIS_DIR/sonic-db/database_config.json
fi

mkdir -p /etc/supervisor/conf.d/
if [[ $NS_ID == "" ]] && [[ $NS_NUM -gt 1 ]]
then
    python -c "import sys;import jinja2;loader = jinja2.FileSystemLoader('/usr/share/sonic/templates/');env = jinja2.Environment(loader=loader, trim_blocks=True);template = env.get_template('database_global.json.j2');print(template.render(PREFIX=sys.argv[1], NS_REF_CNT=sys.argv[2]))" "$NS_PREFIX" "$NS_NUM" > $REDIS_DIR/sonic-db/database_global.json
fi

# generate all redis server supervisord configuration file
sonic-cfggen -j /var/run/redis/sonic-db/database_config.json -t /usr/share/sonic/templates/supervisord.conf.j2 > /etc/supervisor/conf.d/supervisord.conf

exec /usr/bin/supervisord
