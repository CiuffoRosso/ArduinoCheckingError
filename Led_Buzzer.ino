void AccendiLed() {
  if (liv_allerta < 3) //Tutto ok
  {
    Serial.println("Tutto ok");
    LedBicolor_ImpostaStato(VERDE);
  }
  if (liv_allerta == 3) //warning
  {
    Serial.println("Warning!");
    SuonaBuzzer(2, BeepCorto);
    for (int i = 0; i < postingInterval / 2000; i++)
    {
      LedBicolor_ImpostaStato(ROSSO);
      delay(1000);
      LedBicolor_ImpostaStato(SPENTO);
      delay(1000);
    }
  }
  if (liv_allerta > 3) //error fatal
  {
    Serial.println("Error fatal!");
    LedBicolor_ImpostaStato(ROSSO);
    SuonaBuzzer(4, BeepLungo);
  }
}

void LedBicolor_ImpostaStato(byte stato)
{
  if (stato == SPENTO)
  {
    digitalWrite(Led_pin0, 0);
    digitalWrite(Led_pin1, 0);
  }
  else
  {
    stato--;
    digitalWrite(Led_pin0, stato);
    digitalWrite(Led_pin1, 1 - stato);
  }
}

void SuonaBuzzer(int n, int del) {
  if ((millis() - lastBuzzerTime > buzzerInterval) || (prec != liv_allerta)) {
    lastBuzzerTime = millis();
    for (int i = 0; i < n; i++)
    {
      digitalWrite(BuzzerVCC, 1);
      delay(del);
      digitalWrite(BuzzerVCC, 0);
      delay(del);
    }
  }
}

