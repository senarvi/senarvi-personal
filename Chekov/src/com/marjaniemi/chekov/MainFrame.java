/*
 * Copyright (c) 2002 Seppo Enarvi. All Rights Reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * -Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 * 
 * -Redistribution in binary form must reproduct the above copyright
 *  notice, this list of conditions and the following disclaimer in
 *  the documentation and/or other materials provided with the distribution.
 */


package com.marjaniemi.chekov;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.RenderedImage;
import javax.swing.*;
import javax.swing.border.*;
import java.util.logging.Logger;


public class MainFrame extends JFrame
{
    private static Logger logger_ = Logger.getLogger("com.marjaniemi.chekov");

    private ImageDisplayPane mapPane_;

    private String pcsLabelText_           = "               PCS: ";
    private String xLabelText_             = "  PCS X coordinate: ";
    private String yLabelText_             = "  PCS Y coordinate: ";
    private String latitudeLabelText_      = "          Latitude: ";
    private String longitudeLabelText_     = "         Longitude: ";
    private String timeLabelText_          = "               UTC: ";
    private String heightLabelText_        = "            Height: ";
    private String courseLabelText_        = "Course over ground: ";
    private String speedLabelText_         = " Speed over ground: ";
    private String numSatellitesLabelText_ = "Visible satellites: ";

    private Label connectionLabel_ = new Label("Disconnected");
    private Label pcsLabel_ = new Label(pcsLabelText_);
    private Label xLabel_ = new Label(xLabelText_);
    private Label yLabel_ = new Label(yLabelText_);
    private Label latitudeLabel_ = new Label(latitudeLabelText_);
    private Label longitudeLabel_ = new Label(longitudeLabelText_);
    private Label timeLabel_ = new Label(timeLabelText_);
    private Label heightLabel_ = new Label(heightLabelText_);
    private Label courseLabel_ = new Label(courseLabelText_);
    private Label speedLabel_ = new Label(speedLabelText_);
    private Label numSatellitesLabel_ = new Label(numSatellitesLabelText_);


    /**
     * Construct the Chekov application object.
     */
    public MainFrame()
    {
	super("Chekov");

        getAccessibleContext().setAccessibleDescription(
	    "Auto-Navigation Software");
        JOptionPane.setRootFrame(this);

        addWindowListener(new WindowAdapter() {
	    public void windowClosing(WindowEvent e) {
		Chekov.terminate();
	    }
	});

	JPanel panel = new JPanel();
        panel.setLayout(new BorderLayout());
        panel.setBorder(new EtchedBorder());
	panel.add(createSideBar(), BorderLayout.EAST);
        mapPane_ = new ImageDisplayPane("No map loaded yet");
        panel.add(mapPane_, BorderLayout.CENTER);

        getContentPane().removeAll();
        getContentPane().setLayout(new BorderLayout());
        getContentPane().add(panel, BorderLayout.CENTER);
        
        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        setLocation(0, 0);
        setSize(screenSize.width, screenSize.height);
	setUndecorated(true);
        validate();
        repaint();
        setVisible(true);
    }

 
    private Panel createSideBar()
    {
	Panel panel = new Panel();
	panel.setLayout(new BoxLayout(panel, BoxLayout.PAGE_AXIS));
	panel.setBackground(Color.WHITE);

	Button exitButton = new Button("Exit");
	exitButton.addActionListener(new ActionListener()
	    {
		public void actionPerformed(ActionEvent e) {
		    Chekov.terminate();
		}
	    });

	Button connectButton = new Button("Connect");
	final MainFrame thisMainFrame = this;
	connectButton.addActionListener(new ActionListener()
	    {
		public void actionPerformed(ActionEvent e)
		{
		    ConnectDialog connectDialog = new ConnectDialog(
		        thisMainFrame);
		    boolean accepted = connectDialog.showDialog();
		    if (!accepted) return;

		    boolean succeeded = Chekov.nmeacInterface.setup(
		        connectDialog.getHost(),
			connectDialog.getPort());

		    if (succeeded) {
			connectionLabel_.setText(connectDialog.getHost() +
						":" + connectDialog.getPort());
		    }
		    else {
			connectionLabel_.setText("Disconnected");
			JOptionPane.showMessageDialog(
			    thisMainFrame,
			    "Unable to connect to the nmead server.",
			    "Chekov - Error",
			    JOptionPane.ERROR_MESSAGE);
		    }
		}
	    });

	Button disconnectButton = new Button("Disconnect");
	disconnectButton.addActionListener(new ActionListener()
	    {
		public void actionPerformed(ActionEvent e)
		{
		    Chekov.nmeacInterface.close();
		    connectionLabel_.setText("Disconnected");
		}
	    });
	
	Font defaultFont = xLabel_.getFont();
	Font sideBarFont = new Font("Monospaced", Font.PLAIN, 8);

	exitButton.setFont(sideBarFont);
	connectButton.setFont(sideBarFont);
	disconnectButton.setFont(sideBarFont);
	connectionLabel_.setFont(sideBarFont);
	pcsLabel_.setFont(sideBarFont);
	xLabel_.setFont(sideBarFont);
	yLabel_.setFont(sideBarFont);
	latitudeLabel_.setFont(sideBarFont);
	longitudeLabel_.setFont(sideBarFont);
	timeLabel_.setFont(sideBarFont);
	heightLabel_.setFont(sideBarFont);
	courseLabel_.setFont(sideBarFont);
	speedLabel_.setFont(sideBarFont);
	numSatellitesLabel_.setFont(sideBarFont);

	panel.add(exitButton);
	panel.add(connectButton);
	panel.add(disconnectButton);
	panel.add(connectionLabel_);
	panel.add(pcsLabel_);
	panel.add(xLabel_);
	panel.add(yLabel_);
	panel.add(latitudeLabel_);
	panel.add(longitudeLabel_);
	panel.add(timeLabel_);
	panel.add(heightLabel_);
	panel.add(courseLabel_);
	panel.add(speedLabel_);
	panel.add(numSatellitesLabel_);

	return panel;
    }


    /**
     * Change the map that is to be displayed.
     *
     * @param im The new map image to display.
     * @param imageName The name of the map to appear on the
     *        upper left corner of the map.
     */
    public void displayMap(RenderedImage mapImage, String mapName,
			   String pcsName)
    {
	assert mapPane_ != null;
	setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
	mapPane_.set(mapImage, mapName);
        setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));

	pcsLabel_.setText(pcsLabelText_ + pcsName);
	validate();
	repaint();
    }


    public void displayPosition(PlanarCoordinate position)
    {
	int x = (new Double(position.getX())).intValue();
	int y = (new Double(position.getY())).intValue();

	mapPane_.setReference(x, y);
	xLabel_.setText(xLabelText_ + new Integer(x));
	yLabel_.setText(yLabelText_ + new Integer(y));
    }


    public void displayGeographicPosition(double latitude, double longitude)
    {
	char eastOrWest, northOrSouth;
	if (latitude >= 0)
	{
	    northOrSouth = 'N';
	}
	else
	{
	    northOrSouth = 'S';
	    latitude = -latitude;
	}
	if (longitude >= 0)
	{
	    eastOrWest = 'E';
	}
	else
	{
	    eastOrWest = 'W';
	    longitude = -longitude;
	}
	latitude = (double)Math.round(latitude * 100) / 100;
	longitude = (double)Math.round(longitude * 100) / 100;
	latitudeLabel_.setText(latitudeLabelText_ + latitude + ' ' + northOrSouth);
	longitudeLabel_.setText(longitudeLabelText_ + longitude + ' ' + eastOrWest);
    }

    public void displayTime(String time)
    {
	timeLabel_.setText(timeLabelText_ + time);
    }

    public void displayHeight(double height)
    {
	heightLabel_.setText(heightLabelText_ + height);
    }

    public void displayCourse(double course)
    {
	courseLabel_.setText(courseLabelText_ + course);
    }

    public void displaySpeed(double speed)
    {
	speedLabel_.setText(speedLabelText_ + speed);
    }

    public void displayNumSatellites(int numSatellites)
    {
	numSatellitesLabel_.setText(numSatellitesLabelText_ + numSatellites);
	validate();
	repaint();
    }
}
