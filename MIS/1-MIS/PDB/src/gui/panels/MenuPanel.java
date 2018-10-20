package gui.panels;

import enums.Panel;
import gui.MainWindow;
import listeners.*;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GradientPaint;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import javax.swing.BorderFactory;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JToolBar;
import models.Database;

/**
 * @author xstane33
 * @author xskota07
 * @author xklima22
 */
public class MenuPanel extends JToolBar {
    
    JButton connectButton;
    JButton disconnectButton;
    JButton layoutButton;

   
    //private final Image image;
    private final MainWindow mainWindow;
    
    
    public MenuPanel(MainWindow mainWindow) {
        //this.setBorder(BorderFactory.createMatteBorder(0, 0, 1, 0, new Color(50, 50, 50)));
        this.mainWindow = mainWindow;
        this.setLayout();
    }

    @Override
    protected void paintComponent(Graphics g) {
        // Create the 2D copy
        Graphics2D g2 = (Graphics2D) g.create();

        // Apply vertical gradient
        g2.setPaint(new GradientPaint(0, 0, new Color(255, 255, 255), 0,
            getHeight(), new Color(220, 220, 220)));
        g2.fillRect(0, 0, getWidth(), getHeight());

        // get window width
        Dimension size = this.getSize();
        super.paintComponent(g);
        // Dipose of copy
        g2.dispose();
    }
    
    
    private void setLayout() {
        this.setFloatable(false);
        this.setOpaque(false);

        // Connect
        Icon i = getImageIcon("connect.png");
        this.connectButton = new JButton(i);
        this.connectButton.setFocusPainted(false);
        this.connectButton.addActionListener(new MenuPanelConnectListener(this));
        this.connectButton.setToolTipText("Connect to database");
        this.connectButton.setVisible(true);
        this.connectButton.setBorderPainted(false);
        add(this.connectButton, BorderLayout.WEST);
        // Disconnect
        i = getImageIcon("disconnect.png");
        this.disconnectButton = new JButton(i);
        this.disconnectButton.setFocusPainted(false);
        this.disconnectButton.addActionListener(new MenuPanelDisconnectListener(this));
        this.disconnectButton.setToolTipText("Disconnect");
        this.disconnectButton.setVisible(false);
        this.disconnectButton.setBorderPainted(false);
        add(this.disconnectButton, BorderLayout.WEST);
        // Layout Manager
        i = getImageIcon("map.png");
        this.layoutButton = new JButton(i);
        this.layoutButton.setFocusPainted(false);
        this.layoutButton.addActionListener(new MenuPanelLayoutMgrListener(this));
        this.layoutButton.setToolTipText("Layout Manager");
        this.layoutButton.setVisible(true);
        this.layoutButton.setEnabled(false);
        this.layoutButton.setBorderPainted(false);
        add(this.layoutButton, BorderLayout.WEST);
    }
    
    private ImageIcon getImageIcon(String image) {
        return new ImageIcon(getClass().getClassLoader().getResource("resources/images/" + image));
    }
    
    public void setButtonsToConnectState() {
        if(Database.sharedInstance() != null && Database.sharedInstance().isConnected())
        {
            this.connectButton.setVisible(false);
            this.disconnectButton.setVisible(true);
            this.layoutButton.setEnabled(true);
            LayoutManagerPanel ws = new LayoutManagerPanel();
            MainWindow.getMainWindow().setWorkspace(ws, Panel.LAYOUT_MANAGER);
        }
    }

    public void setButtonsToNotConnectState() {
        this.connectButton.setVisible(true);
        this.disconnectButton.setVisible(false);
        this.layoutButton.setEnabled(false);
    }
    public MainWindow getMainWindow() {
        return this.mainWindow;
    }
}