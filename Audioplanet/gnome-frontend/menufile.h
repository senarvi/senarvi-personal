//  menufile.h
//  Copyright (C) 1999-2000 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef MENUFILE_H
#define MENUFILE_H

#ifdef __GNUG__
#pragma interface
#endif


namespace Menu {


namespace File {


extern void newAudioClip();
extern void open();
extern void save();
extern void saveAs();
extern void quit();


}  // namespace File


}  // namespace Menu


#endif  // MENUFILE_H

// Local Variables:
// mode:C++
// End:
