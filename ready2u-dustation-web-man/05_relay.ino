void relaySetup()
{
    pinMode(R1, OUTPUT);

}

void relayHi(int relay){

    digitalWrite(relay, HIGH);
}

void relayLo(int relay){
    digitalWrite(relay, LOW);
}