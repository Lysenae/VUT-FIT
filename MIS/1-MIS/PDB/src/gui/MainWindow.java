package gui;

import enums.Panel;
import javax.swing.JFrame;
import gui.panels.*;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Container;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.sql.SQLException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JPanel;

/**
 * Main application window
 * @author xstane33
 * @author xskota07
 * @author xklima22
 */
public class MainWindow extends JFrame  {
    private Panel currentWorkspacePanel;

    private JPanel workspacePanel;
    private MenuPanel menuPanel;
    
    public static MainWindow wnd;
    
    /**
     * Class constructor.
     */
     public MainWindow() {
        this.setupLayout();
        this.currentWorkspacePanel = Panel.LOGIN;
        wnd = this;
    }
     
    /**
     * Create layout.
     */
    private void setupLayout(){
        this.setLocationRelativeTo(null);
        this.setBackground(new Color(100, 100, 100));
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setTitle("PDB");

        Container pane = this.getContentPane();
        
        // set GB Layout
        pane.setLayout(new GridBagLayout( ));
        pane.setBackground(Color.red);
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.fill = GridBagConstraints.BOTH;
       
        // add menuBar
        this.menuPanel = new MenuPanel(this);
        gbc.gridx      = 0;
        gbc.gridy      = 0;
        gbc.weightx    = 0;
        gbc.ipady      = 5;
        pane.add(menuPanel, gbc);
        
        gbc.gridy++;
        gbc.weighty = 99;
        gbc.weightx = 99;

        this.workspacePanel = new JPanel();
        this.workspacePanel.setLayout(new BorderLayout());
        pane.add(workspacePanel, gbc);
    }
    
    /**
     * Get instance of main window.
     * @return main window instance
     */
    public static MainWindow getMainWindow()
    {
        return wnd;
    }
    
    /**
     * Redraw window.
     */
    public static void redraw()
    {
        if(wnd != null)
        {
            wnd.workspacePanel.validate();
            wnd.workspacePanel.repaint();
        }
    }
    
    /**
     * Sets current workspace panel.
     * @param ws workspace panel
     * @param type panel type 
     */
    public void setWorkspace(JPanel ws, Panel type)
    {
        if(this.currentWorkspacePanel != type)
        {
            this.clearWorkspace();
            this.currentWorkspacePanel = type;
            this.workspacePanel.add(ws, BorderLayout.CENTER);
            this.workspacePanel.validate();
            this.workspacePanel.repaint();
        }
    }
    
    /**
     * Get current workspace panel.
     * @return workspace panel
     */
    public JPanel getWorkspace()
    {
        return this.workspacePanel;
    }
    
    /**
     * Remove all components from workspace.
     */
    public void clearWorkspace()
    {
        this.workspacePanel.removeAll();
    }
}
