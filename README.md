# Auto_Reboot_Modem_esp8266
Script that will cycle a relay if it's unable to ping a website.
This is using an ESP-01 relay board.

Backstory - For about the last year my isp is having issues in my area.  I have had several techs out to look at the issue and have not be able to resolve it.  They insist the issue is upstream from my house and they will have a line tech check upstream.  This has gone on for about 8 months now and there are no other isps in my area that are any good.  The issue seems to be that the upstream is week and I start getting extreme packet loss.  When this happens I can restart the modem and it corrects the issue 99.9% of the time.  I decided there should be a way to automate this redundent task.

This arduino script will ping a website to check if it receives a response back or not.  If it fails it waits for 5 failures before it restarts to make sure that we are proper down and not just a random time out or two.

This project is based on the one I found here:  http://www.whatimade.today/make-an-automatic-router-re-starter-for-3-with-an-esp8266-01-and-single-relay/
However it seemed to be just doing a dns lookup to verify if the connection was down.  I made some adjustments to this at first but it was not consitent on timing when it thought the connection was down.  I ended up adding  esp8266Ping.h (https://github.com/dancol90/ESP8266Ping )to the project and using it's ping capablilties instead of just a name lookup.  