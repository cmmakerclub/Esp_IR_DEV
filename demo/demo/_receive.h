PubSubClient::callback_t on_message_arrived =
[&](const MQTT::Publish & pub) -> void {
    String topic = pub.topic();
    String payload = pub.payload_string();
    String text = topic + " => " + payload;

    Serial.println(text);

    if (payload == "ON") {
      Serial.println(">> ON");
      eep_ir_load(1);  // turn on
      IR_transmit();
      delay(50);
      eep_ir_load(1);  // turn on
      IR_transmit();
    }
    else if (payload == "OFF") {
      Serial.println(">> OFF");
      eep_ir_load(0);  // turn off
      IR_transmit();
      delay(50);
      eep_ir_load(0);  // turn on
      IR_transmit();
    }
 };
