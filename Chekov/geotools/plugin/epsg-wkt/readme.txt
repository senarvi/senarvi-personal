
GeoTools Plugin/Extention : 
  EPSG and AUTO CRSAuthoritySpi are defined for data.crs.CRSService.
  These codes are used to provide CoordinateRegerenceSystem for GCE and
  DataStore implementations that do not define their own native definition.
  
  Contains a Property file based CoordinateSystemAuthority (the property file is too
  large to be included in the main geotools release). This class will need to be
  ported to the new GeoAPI CoordinateReferenceSystem interfaces.

To use this plugin/extention ensure that gt2-epsg-wkt.jar is on your classpath.
If this directory contains a lib folder then the jars it contains must also be included.

Please report bugs and feature requests using http://jira.codehaus.org/secure/BrowseProject.jspa?id=10270.                
