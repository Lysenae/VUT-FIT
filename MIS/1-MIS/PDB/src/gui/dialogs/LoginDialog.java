// xstane33, Jiří Staněk
// xskota07, Jiří Skoták
// xklima22, Daniel Klimaj

package gui.dialogs;
import gui.panels.*;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;
import javax.swing.JCheckBox;
/**
 * @author xskota07
 * @author xstane33
 */
public class LoginDialog extends JPanel {
    private final MenuPanel menuPanel;
    private final JPanel popupPanel;
    // Fields
    public JTextField loginField;
    public JTextField passwordField;
    // Labels
    private final JLabel loginLabel;
    private final JLabel passwordLabel;
    // Check Box
    public JCheckBox initDbCheckBox;

    /**
     * Class constructor
     * @param menuPanel parent
     */
    public LoginDialog(MenuPanel menuPanel) {
        
        this.menuPanel = menuPanel;
        // create panel
        this.popupPanel = new JPanel();
        // create fields
        this.loginField = new JTextField();
        this.passwordField = new JPasswordField();
        //create checkbox
        this.initDbCheckBox = new JCheckBox("Init database", false);
        // create labels
        this.loginLabel = new JLabel("Login");
        this.passwordLabel = new JLabel("Password");
        
        
        // set Layout
        this.add(this.popupPanel);
        this.popupPanel.setLayout(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();

        // Login label
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.ipadx = 40;
        gbc.weightx = 0.5;
        this.popupPanel.add(this.loginLabel, gbc);

        // Login field
        gbc.gridx = 1;
        gbc.gridy = 0;
        gbc.ipadx = 150;
        gbc.weightx = 0.5;
        this.popupPanel.add(this.loginField, gbc);

        // Password label
        gbc.gridx = 0;
        gbc.gridy = 1;
        gbc.ipadx = 40;
        gbc.weightx = 0.5;
        this.popupPanel.add(this.passwordLabel, gbc);

        // Password field
        gbc.gridx = 1;
        gbc.gridy = 1;
        gbc.ipadx = 150;
        gbc.weightx = 0.5;
        this.popupPanel.add(this.passwordField, gbc);
        
        // Checkbox init database
        gbc.gridx = 0;
        gbc.gridy = 2;
        gbc.gridwidth = 2;
        gbc.ipadx = 20;
        this.popupPanel.add(this.initDbCheckBox, gbc);
    }
}
