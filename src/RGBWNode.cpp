/*
 * RGBWNode.cpp
 *
 *  Created on: 16.05.2016
 *      Author: ian
 */

#include "RGBWNode.h"
#include "Homie.hpp"


const float RGBWNode::percent_to_pwm = PWMRANGE / 100;

RGBWNode::RGBWNode() : 	HomieNode("LEDs", "RGBW") {
	_subscribeToAll = true;
}


bool RGBWNode::InputHandler(String property, String value) {
	Serial.printf("Striphandler received  property %s (value=%s).\n", property.c_str(), value.c_str());
	int value_int = value.toInt();
	uint_fast8_t id = R;
	for (id=R; id <= W; id++)
		if (property[0] == rgbw_id[id]) break;
	if (id>W) return false;

	rgbw_values[id] = round(value_int * percent_to_pwm);
	updateLED(id);
	return true;
}

void RGBWNode::updateLEDs() const {
	for (uint_fast8_t i=0;i<4;i++) updateLED(i);
}

void RGBWNode::updateLED(uint8_t id) const {
	if (id < R || id > W) return;
	uint16_t value = rgbw_values[id];
	uint8_t pin=rgbw_pins[id];
	Serial.printf("Update LED %c on Pin %d, value %d.\n", rgbw_id[id], pin, value);
	analogWrite(pin, value);
	PublishState(id);
}

void RGBWNode::PublishState(uint8_t id) const {
	if (id < R || id > W) return;
	String id_string(rgbw_id[id]);
	String value_string(rgbw_values[id]);
	Homie.setNodeProperty(*this, id_string, value_string,true);
}

void RGBWNode::setup() {
	for (uint_fast8_t i=0;i<4;i++)
	{
		pinMode(rgbw_pins[i], OUTPUT);
	}
	updateLEDs();
}
