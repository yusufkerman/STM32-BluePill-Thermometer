#include <Peripherals/UsartDriver.h>




void UsartDriver::init(TxOutputSpeed txSpeed,uint32_t clk, uint32_t baudrate){

	this->clk = clk;
	this->baudrate = baudrate;

	// USART clock enable
	if(this->usart == USART1) {
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	} else if(this->usart == USART2) {
	    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	} else if(this->usart == USART3) {
	    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	}


	for (UsartPinMap item : usartMap){
		if (item.usart == this->usart){
			GPIO_TypeDef* port = item.port;

			//Enable clock access for relevant gpio port
			gpio->enableClock(port);

			//Configure tx and rx pins.
			gpio->setPinMode(port, item.tx.pin, mapTxSpeedToPinMode(txSpeed), PinConfig::AFOutputPP);
			gpio->setPinMode(port, item.rx.pin, PinMode::Input, PinConfig::FloatingInput);

			//Configure the baudrate
			uart_set_baudrate(usart,clk,baudrate);

			// Enable transmitter & receiver
			usart->CR1 |= USART_CR1_TE | USART_CR1_RE;
			// Enable USART
			usart->CR1 |= USART_CR1_UE;

		}
	}

}
PinMode UsartDriver::mapTxSpeedToPinMode(TxOutputSpeed speed) {
    switch(speed) {
        case TxOutputSpeed::Output10MHz: return PinMode::Output10MHz;
        case TxOutputSpeed::Output2MHz:  return PinMode::Output2MHz;
        case TxOutputSpeed::Output50MHz: return PinMode::Output50MHz;
    }
    return PinMode::Output2MHz; // default
}
void UsartDriver::write(uint8_t data) {
    // TXE = Transmit Data Register Empty
    while (!(usart->SR & USART_SR_TXE)) {
        // Polling...
    }

    // Write to data register.
    usart->DR = data;
}
void UsartDriver::writeString(const std::string& str) {
    for(char c : str) {
        write(static_cast<uint8_t>(c));
    }
}
void UsartDriver::enableInterrupt(IRQn_Type irqNumber){

	 // RX interrupt enable
	 usart->CR1 |= USART_CR1_RXNEIE;  // RX buffer not empty interrupt

	 // NVIC enable
	 NVIC_EnableIRQ(irqNumber);

}
uint16_t UsartDriver::calculate_baudrate(uint32_t clockFreq, uint32_t baudrate){
	float usartdiv = (float)clockFreq / (16.0f * baudrate);
	uint16_t mantissa = (uint16_t)usartdiv;
	uint16_t fraction = (uint16_t)((usartdiv - mantissa) * 16);
	return (mantissa << 4) | (fraction & 0xF);
}
void UsartDriver::uart_set_baudrate(USART_TypeDef *USARTx, uint32_t clockFreq, uint32_t baudrate){
	USARTx->BRR = calculate_baudrate(clockFreq,baudrate);
}
static void IRQHandlerStatic(UsartDriver* driver) {
	if(driver->rxCallback) {
		char c = driver->getUsart()->DR;
        driver->rxCallback(c);
    }
}
