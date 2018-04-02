/*
 * Copyright (c) 2003 Seppo Enarvi. All Rights Reserved.
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
import javax.swing.*;
import java.util.prefs.*;


public class ConnectDialog extends JDialog implements ActionListener
{
    /* Preferences API node. */
    private static final String PREFERENCES_NODE = "/com/marjaniemi/chekov";

    /* Preferences API key. */
    private static final String PREF_DEFAULT_NMEAD_HOST = "default_nmead_host";
    private static final String PREF_DEFAULT_NMEAD_PORT = "default_nmead_port";

    /* <code>true</code>, if Ok button has been pressed. */
    private boolean accepted_ = false;

    /**
     * The given domain name or "localhost" if the user has not clicked the
     * Ok button.
     */
    private String host_ = "localhost";

    /**
     * The given port number or 1155 if the user has not clicked the Ok
     * button.
     */
    private int port_ = 1155;

    JTextField hostField_ = null;
    JTextField portField_ = null;
    JButton okButton_ = new JButton("Ok");
    JButton cancelButton_ = new JButton("Cancel");


    public ConnectDialog(JFrame frame)
    {
	super(frame, "Chekov - Connect", true);


	Preferences prefs = Preferences.userRoot().node(PREFERENCES_NODE);
	host_ = prefs.get(PREF_DEFAULT_NMEAD_HOST, host_);
	port_ = prefs.getInt(PREF_DEFAULT_NMEAD_PORT, port_);


	JTextField text = new JTextField("Enter the domain name and the " +
					 "port number of the NMEA daemon.");
	text.setEditable(false);

	Label hostLabel = new Label("Host: ");
	Label portLabel = new Label("Port: ");
	hostField_ = new JTextField(host_);
	portField_ = new JTextField("" + port_);
	hostField_.setEditable(true);
	portField_.setEditable(true);

	okButton_.addActionListener(this);
	cancelButton_.addActionListener(this);
	okButton_.setSelected(true);

	JPanel inputPanel = new JPanel();
	inputPanel.setLayout(new GridLayout(2, 0));
	inputPanel.add(hostLabel);
	inputPanel.add(hostField_);
	inputPanel.add(portLabel);
	inputPanel.add(portField_);

	JPanel buttonPanel = new JPanel();
	buttonPanel.setLayout(new FlowLayout());
	buttonPanel.add(okButton_);
	buttonPanel.add(cancelButton_);

	getContentPane().setLayout(new BorderLayout());
	getContentPane().add(text, BorderLayout.NORTH);
	getContentPane().add(inputPanel, BorderLayout.CENTER);
	getContentPane().add(buttonPanel, BorderLayout.SOUTH);


	//pack();
	setSize(410, 150);
        validate();
        repaint();
    }


    public void actionPerformed(ActionEvent e)
    {
	assert hostField_ != null;
	assert portField_ != null;


	if (e.getSource().equals(okButton_))
	{
	    try {
		port_ = Integer.parseInt(portField_.getText());
	    }
	    catch (NumberFormatException nfe) {
		JOptionPane.showMessageDialog(this,
					      "Invalid port number.",
					      "Chekov - Error",
					      JOptionPane.ERROR_MESSAGE);
		return;
	    }
	    host_ = hostField_.getText();
	    
	    Preferences prefs = Preferences.userRoot().node(PREFERENCES_NODE);
	    prefs.put(PREF_DEFAULT_NMEAD_HOST, host_);
	    prefs.putInt(PREF_DEFAULT_NMEAD_PORT, port_);

	    setVisible(false);
	    accepted_ = true;
	}
	else if (e.getSource().equals(cancelButton_))
	{
	    setVisible(false);
	}
	else
	{
	    assert false;
	}
    }


    /**
     * Display the dialog and wait for user input. After this method returns,
     * if the user clicked Ok button, the input the user gave can be retrieved
     * with the <code>getHost</code> and <code>getPort</code> methods.
     *
     * @return If the user clicked the Ok button, return <code>true</code>,
     *         otherwise return <code>false</code>.
     */ 
    public boolean showDialog()
    {
	setVisible(true);
	return accepted_;
    }


    public String getHost() { return host_; }
    public int getPort() { return port_; }
}
