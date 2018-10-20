
package listeners;
import enums.Panel;
import gui.MainWindow;
import gui.panels.LayoutManagerPanel;
import gui.panels.MenuPanel;
import gui.dialogs.LoginDialog;
import models.Database;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.SQLException;
import javax.swing.JOptionPane;
/**
 * @author xstane33
 * @author xskota07
 * @author xklima22
 */
public class MenuPanelConnectListener implements ActionListener{
    private final MenuPanel menuPanel;

    public MenuPanelConnectListener(MenuPanel menuPanel) {
        this.menuPanel = menuPanel;
    }
    
    @Override
    public void actionPerformed(ActionEvent e) {
        // Dialog for login and password
        LoginDialog loginDialog = new LoginDialog(this.menuPanel);
        Object[] options = {"Login"};
        this.menuPanel.getMainWindow();

                // Change buttons
   
        // Show dialog
        int i = JOptionPane.showOptionDialog(this.menuPanel.getMainWindow(), loginDialog, "Login",
                JOptionPane.PLAIN_MESSAGE,JOptionPane.PLAIN_MESSAGE,null,options,options[0]);
        if (i == 0) {
            try {
                
                // Connect
                Database.sharedInstance().connect(loginDialog.loginField.getText(),
                    loginDialog.passwordField.getText());
                //Init DB
                if (loginDialog.initDbCheckBox.isSelected()) {
                Database.sharedInstance().runSQLScript("init.sql");
                }
                // Set connected state
                this.menuPanel.setButtonsToConnectState();
                //this.menuPanel.setConnected(true);
            }
          
            catch (SQLException ex) {
                if(ex.getErrorCode() == 1017) {
                    JOptionPane.showMessageDialog(this.menuPanel.getMainWindow(),
                        "Invalid username or password!.", "Invalid username or password!",
                        JOptionPane.ERROR_MESSAGE);
                }
                else {
                    JOptionPane.showMessageDialog(this.menuPanel.getMainWindow(),
                        ex.getMessage(), "SQLException", JOptionPane.ERROR_MESSAGE);
                }
                return;
            }
            LayoutManagerPanel ws = new LayoutManagerPanel();
            MainWindow.getMainWindow().setWorkspace(ws, Panel.LAYOUT_MANAGER);
        }
    }
}
