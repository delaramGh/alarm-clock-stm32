from kivy.app import App
from kivy.uix.gridlayout import GridLayout
from kivy.uix.label import Label
from kivy.uix.textinput import TextInput
from kivy.uix.button import Button 
from kivy.uix.widget import Widget



class myWindow(Widget):
    pass
    # def __init__(self, **kwargs):
    #     super(myWindow, self).__init__(**kwargs)
    #     self.cols = 1

    #     self.layout_1 = GridLayout()
    #     self.layout_1.cols = 2
    #     self.layout_1.add_widget(Label(text='User Name'))
    #     self.username = TextInput(multiline=False)
    #     self.layout_1.add_widget(self.username)
    #     self.layout_1.add_widget(Label(text='password'))
    #     self.password = TextInput(password=True, multiline=False)
    #     self.layout_1.add_widget(self.password)
    #     self.layout_1.add_widget(Label(text="Email: "))
    #     self.email = TextInput()
    #     self.layout_1.add_widget(self.email)

    #     self.add_widget(self.layout_1)

    #     self.ok = Button(text="ok")
    #     self.ok.bind(on_press=self.ok_callback)
    #     self.add_widget(self.ok)

    # def ok_callback(self, instance):
    #     print("okay button! :)")

class MyApp(App):
    def build(self):
        return myWindow()


if __name__ == '__main__':
    MyApp().run()

