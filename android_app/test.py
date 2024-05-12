from kivy.app import App
from kivy.uix.gridlayout import GridLayout
from kivy.uix.label import Label   
from kivy.uix.textinput import TextInput
from kivy.uix.button import Button 
from kivy.uix.widget import Widget
from kivy.properties import ObjectProperty



class myWindow(Widget):
    def __init__(self, **kwargs):
        super(myWindow, self).__init__(**kwargs)

        self.ok = Button(text="ok")
        self.ok.bind(on_press=self.ok_callback)
        self.add_widget(self.ok)

    def ok_callback(self, instance):
        print("okay button! :)")
        

class MyApp(App):
    def build(self):
        return myWindow()


if __name__ == '__main__':
    MyApp().run()

