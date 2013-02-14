CREATE TABLE feedback (
    id serial NOT NULL,
    time timestamp DEFAULT current_timestamp NOT NULL,
    type text DEFAULT "" NOT NULL,
    account text,
    email text,
    player text,
    room integer,
    subject text,
    message text
);
