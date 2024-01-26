import SqrtDecompositions.SqrtDecompos;

import javax.swing.*;
import javax.swing.text.*;
import java.awt.*;
import java.awt.event.*;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.Map;
import java.util.Vector;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static javax.swing.GroupLayout.Alignment.BASELINE;
import static javax.swing.GroupLayout.Alignment.LEADING;


class JTextFieldLimit extends PlainDocument {
    private int limit;
    private int maxNumber;
    JTextFieldLimit(int limit,int maxNumber) {
        super();
        this.limit = limit;
        this.maxNumber = maxNumber;
    }
    JTextFieldLimit(int limit, boolean upper) {
        super();
        this.limit = limit;
    }
    public void insertString(int offset, String str, AttributeSet attr) throws BadLocationException {
        if (str == null)
            return;
        if ((getLength() + str.length()) <= limit) {
            if (isNumber(str)) {
                int number = Integer.parseInt(getText(0, getLength()) + str);
                if (number <= maxNumber) {
                    super.insertString(offset, str, attr);
                }
            }
        }
    }
    private boolean isNumber(String string) {
        Pattern pattern = Pattern.compile("[0-9]+$");
        Matcher matcher = pattern.matcher(string);
        return matcher.matches();
    }
}
public class Main extends JFrame  {
    SqrtDecompos sqrtDecompos = new SqrtDecompos();
    JLabel Label_Array;
    JLabel Label_Min;
    JTextField Text_Left;
    JLabel Text_Min;
    JTextField Text_Right;
    JLabel Label_Search;
    JButton Button_Random;
    JButton Button_Update;
    JButton Button_Search;
    Color UpdateColor = Color.orange;
    Color SelectColor = Color.white;
    Color StandColor = Color.LIGHT_GRAY;

    ArrayList< JTextField> MasElement;
    public Main()
    {
        JFrame frame = new JFrame("Сортировка методом SQRT-декомпозиции");
        frame.setDefaultCloseOperation( EXIT_ON_CLOSE );
        frame.setSize(800, 800);
        frame.setResizable(false);
        Container container = frame.getContentPane();

        JPanel flow = new JPanel(new FlowLayout(FlowLayout.CENTER));
        JPanel flowInput = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JPanel gridInput = new JPanel(new GridLayout(4, 3, 20, 10) );
        JPanel flowArray = new JPanel(new FlowLayout(FlowLayout.LEFT));

        gridInput.setPreferredSize(new Dimension(745,100));
        flowInput.setPreferredSize(new Dimension(750,120));
        flowArray.setPreferredSize(new Dimension(750,750));


        JLabel Label_CountArray = new JLabel("Введите кол-во элементов (макс. 200)");
        JLabel Label_Plug = new JLabel("");
        JTextField Text_CountArray = new JTextField(6);
        JButton Button_CountArray= new JButton("Инициализация");
        Button_Random= new JButton("Заполнить рандомно");
        Button_Update= new JButton("Обновить данные");
        Button_Search= new JButton("Поиск минимальных");
        Label_Array = new JLabel("Массив");
        Label_Search = new JLabel("Введите диапозон поиска(от 0 до N)");
        Label_Min = new JLabel("Мини-ное число в диапозоне(от 0 до N)");
        Text_Min = new JLabel("0");
        Text_Left = new JTextField(6);
        Text_Right = new JTextField(6);

        gridInput.add( Label_CountArray);
        gridInput.add(Text_CountArray);
        gridInput.add(Button_CountArray);
        gridInput.add(Label_Search);
        gridInput.add(Text_Left);
        gridInput.add(Text_Right);
        gridInput.add(Label_Min );
        gridInput.add(Text_Min);
        gridInput.add(new JLabel(""));
        gridInput.add(Button_Random);
        gridInput.add(Button_Update);
        gridInput.add(Button_Search);

        Text_Right.setDocument(new JTextFieldLimit(6,199));
        Text_Left.setDocument(new JTextFieldLimit(6,199));
        Text_CountArray.setDocument(new JTextFieldLimit(6,200));
        Label_Array.setVisible(false);
        Text_CountArray.setText(Integer.toString(0));
        Text_Left.setText(Integer.toString(0));
        Text_Right.setText(Integer.toString(0));


        Button_CountArray.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {
                flowArray.removeAll();
                flowArray.revalidate();
                flowArray.repaint();
                frame.revalidate();
                sqrtDecompos.Delete();
                int N = Integer.parseInt(Text_CountArray.getText());
                Label_Search.setText("Введите диапозон поиска(от 0 до "+Integer.toString(N-1)+")");
                Label_Min.setText("Мини-ное число в диапозоне(от 0 до "+Integer.toString(N-1)+")");
                Text_Left.setText("0");
                Text_Right.setText("0");


                sqrtDecompos.Init(N);
                sqrtDecompos.Init(N);
                sqrtDecompos.FillZero();
                MasElement = new ArrayList<>();
                for (int i =0;i<N;i++) {
                    JTextField Text_Element = new JTextField(Integer.toString(i),6);
                    Text_Element.setDocument(new JTextFieldLimit(10,99999));
                    Text_Element.setText(Integer.toString(0));
                    sqrtDecompos.Input(i,0);
                    flowArray.add(Text_Element);
                    Text_Element.setBackground(StandColor);
                    MasElement.add(Text_Element);
                    Text_Element.addFocusListener(new FocusAdapter() {
                        @Override
                        public void focusGained(FocusEvent e) {
                            JTextField focus = (JTextField)e.getSource();
                            focus.setBackground(SelectColor);
                            focus.setText("");
                        }

                        @Override
                        public void focusLost(FocusEvent e) {
                            JTextField focus = (JTextField)e.getSource();
                            focus.setBackground(UpdateColor);
                            if(focus.getText().length() == 0){
                                focus.setText("0");
                            }
                        }
                    });
                }
                frame.revalidate();
                Label_Array.setVisible(true);
                ActiveElement(true);

            }
        });
        Button_Random.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {
                sqrtDecompos.FillRandom(0,99999);
                for (int i =0;i<sqrtDecompos.GetLengArray();i++) {
                    JTextField element = MasElement.get(i);
                    element.setText(Integer.toString((int)sqrtDecompos.Output(i)));
                    element.setBackground(StandColor);
                }
                frame.revalidate();
                Label_Array.setVisible(true);
                ActiveElement(true);

            }
        });
        Button_Update.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {
                for (int i =0 ;i<sqrtDecompos.GetLengArray();i++){
                    JTextField element = MasElement.get(i);
                    element.setBackground(StandColor);
                    sqrtDecompos.Input(i,Double.parseDouble(element.getText()));

                }
            }
        });
        Button_Search.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {
                int left = Integer.parseInt(Text_Left.getText());
                int right = Integer.parseInt(Text_Right.getText());
                Text_Left.setText(Integer.toString(Math.min(left,right)));
                Text_Right.setText(Integer.toString(Math.max(left,right)));
                double min = sqrtDecompos.SearchMinStandart(Math.min(left,right),Math.max(left,right));
                Text_Min.setText(Integer.toString((int)min));
            }
        });

        flowInput.add(gridInput);
        flow.add(flowInput);
        flow.add(Label_Array);
        flow.add(flowArray);
        container.add(flow);
        ActiveElement(false);
        frame.setVisible(true);

    }
    public static void main(String[] args) {
        new Main();
    }
    public void ActiveElement(boolean flag){
        Button_Random.setVisible(flag);
        Button_Update.setVisible(flag);
        Button_Search.setVisible(flag);
        Text_Left.setVisible(flag);
        Text_Right.setVisible(flag);
        Label_Search.setVisible(flag);
        Text_Min.setVisible(flag);
        Label_Min.setVisible(flag);
    }
}

