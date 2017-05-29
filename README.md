# PortalPlugin
A collection of portal mechanics. This repository might occasionally be updated but there is generally no support for it.

Contained are three example maps:
* PlanePortals: Rectangular portals that act like those in the game portal. Things going in one end come out the other with their speed and orientation transformed by the exit portal.
* TeleportRoom: A room that looks like a doorway to another open area but teleports everything entering just like the rectangular porals.
* VRPortals: Same as TeleportRoom, but the portal renders for each eye making it appear 3D.

# Usage
You need Visual Studio / XCode. Download the project and open it up. It should compile on its own. Open the maps and hit play, for the VR map you need to play in VR mode.

# Issues
#1 Occasionally the rendered projection will glitch out, resize the window to fix it. 
#2 For some reason using a VR portal and planar portal in the same map or during the same editor session breaks projection. Restart the editor to fix.

# Thanks
AgentMilkshake1 for laying the groundwork in getting a screen space projection for VR.
https://answers.unrealengine.com/questions/234597/screenspace-portals-on-vr.html

Andrew Hurley for explaining how to get a left/right vr mask into a material
https://answers.unrealengine.com/questions/589242/vr-instanced-stereo-breaks-screenposition-material.html

