#ifndef __pbd_gtkmm_pix_h__
#define __pbd_gtkmm_pix_h__

#include <string>
#include <map>
#include <vector>
#include <gtk--.h>

namespace Gtkmmext {

class Pix
{
	typedef std::map<std::string, Pix *> PixCache;
	static PixCache *cache;

	PixCache::iterator cache_position;
	int refcnt;
        bool generated;
	std::vector<std::string *> *files;
	std::vector<const char **>   data;
	bool from_files;
	size_t pixmap_count;
	size_t last_pixmap;
	GdkPixmap **pixmaps;
	GdkBitmap **bitmaps;
	size_t max_pixwidth;
	size_t max_pixheight;
	bool _homegenous;

        Pix (const std::string &dirpath, const std::string &regexp, 
	     bool homogenous = true);
	Pix (std::vector<const char **> xpm_data, bool homogenous = true);
        virtual ~Pix();

	friend  Pix *get_pix (const std::string &dirpath, 
			      const std::string &regexp,
			      bool homogenous);
	friend  Pix *get_pix (std::string name, 
			      std::vector<const char **> xpm_data,
			      bool homogenous);
	friend  void finish_pix (Pix *);

  public:      
	Pix (bool homogenous = true);

        void generate (GdkWindow *, GdkColor *);
	size_t n_pixmaps() { return pixmap_count; }
	size_t max_pixmap() { return last_pixmap; }
	bool homogenous () { return _homegenous; }

	/* ref/unref should really be protected, but we don't know the
	   name of the class that should have access to them.  
	*/

	void ref () { refcnt++; }
	void unref () { if (refcnt) refcnt--; }

        GdkBitmap *shape_mask (size_t n) {
		if (n < pixmap_count) {
			return bitmaps[n];
		} 
		return 0;
	}

	GdkPixmap *pixmap(size_t n) {
		if (n < pixmap_count) {
			return pixmaps[n];
		} 
		return 0;
	}

	size_t max_width() { return max_pixwidth; }
	size_t max_height() { return max_pixheight; }
};

extern Pix *get_pix (const std::string &dirpath, 
		     const std::string &regexp, 
		     bool homog = false);

extern Pix *get_pix (std::string, 
		     std::vector<const char **>,
		     bool homog = false);
extern void finish_pix (Pix *);

} /* namespace */

#endif  // __pbd_gtkmm_pix_h__
