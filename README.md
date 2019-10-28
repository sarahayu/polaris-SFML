# Polaris
Planetarium program to render stars, planets, moon, and sun based on location on Earth, local time, and brightness of stars.
Features include:

 - Choosing location based on city or coordinates, utilizing an extensive database of cities pulled from the Internet
 - Change brightness of stars to simulate low-light or city-light settings
 - Stars are properly colored based on their temperature
 - Display name of stars that have them
 - No Internet connection needed—uses purely math to calculate orientation of celestial bodies, planets included
 - Moon phase is properly shown, excluding rotation (will fix maybe sometime in the future)

I made this program so I would be able to locate constellations in bright-light settings, but ended up making the program to be as realistic as possible to the night sky and having the program use purely math instead of fetching information via Internet connection as a challenge for myself.

Screenshots below:
![Program GUI](https://github.com/sarahayu/polaris-SFML/blob/master/screenshots/interface.png?raw=true)

*Program GUI*

![San Francisco in bright-light conditions](https://github.com/sarahayu/polaris-SFML/blob/master/screenshots/sf-bright-light.png?raw=true)

*San Francisco in bright-light condition*

![San Francisco in low-light condition](https://github.com/sarahayu/polaris-SFML/blob/master/screenshots/sf-low-light-conditions.png?raw=true)

*San Francisco in low-light condition*

TODO: add libraries used
