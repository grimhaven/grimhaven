CREATE TABLE room (
    vnum integer NOT NULL,
    owner varchar(80) DEFAULT '' NOT NULL,
    block integer NOT NULL,
    x integer NOT NULL,
    y integer NOT NULL,
    z integer NOT NULL,
    name varchar(127) NOT NULL,
    description text NOT NULL,
    room_flag integer NOT NULL,
    sector integer NOT NULL,
    teletime integer NOT NULL,
    teletarg integer NOT NULL,
    telelook integer NOT NULL,
    river_speed integer NOT NULL,
    river_dir integer NOT NULL,
    capacity integer NOT NULL,
    height integer NOT NULL,
    spec integer NOT NULL
);
