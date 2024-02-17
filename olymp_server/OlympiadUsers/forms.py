from django import forms
from django.forms.widgets import NumberInput
from django.contrib.auth.models import User
from django.contrib.auth.forms import UserCreationForm

from .models import *

class SignUpForm(UserCreationForm):
    class Meta:
        model = User
        fields = ('username', 'password1', 'password2')

    username = forms.CharField(
        label = 'Придумайте имя пользователя',
        max_length = 200,
        required = False,
        widget = forms.TextInput(attrs={'class': 'form-control', 'placeholder': 'Username'})
    )
    password1 = forms.CharField(
        label = 'Придумайте пароль',
        help_text = "(не менее 8 символов, не только цифры)",
        required = False,
        widget = forms.PasswordInput(attrs={'class': 'form-control', 'placeholder': 'Password'})
    )
    password2 = forms.CharField(
        required = False,
        label = 'Повторите пароль',
        widget = forms.PasswordInput(attrs={'class': 'form-control', 'placeholder': 'Password again'})
    )

class SignInForm(forms.Form):

    uname = forms.CharField(
        label = 'Введите имя пользователя',
        max_length = 200,
        required = False,
        widget = forms.TextInput(attrs={'class': 'form-control', 'placeholder': 'Username'})
    )
    pwd = forms.CharField(
        label = 'Введите пароль',
        required = False,
        widget = forms.PasswordInput(attrs={'class': 'form-control', 'placeholder': 'Password'})
    )

class UserForm(forms.ModelForm):
    class Meta:
        model = User
        fields = ('first_name', 'last_name', 'email')

    first_name = forms.CharField(
        label = 'Укажите имя',
        max_length=20,
        required = False,
        widget = forms.TextInput(attrs={'class': 'form-control'})
    )

    last_name = forms.CharField(
        label = 'Укажите фамилию',
        max_length=20,
        required = False,
        widget = forms.TextInput(attrs={'class': 'form-control'})
    )

    email = forms.EmailField(
        label = 'Укажите email',
        required = False,
        widget = forms.EmailInput(attrs={'class': 'form-control'})
    )

class ProfileForm(forms.ModelForm):
    class Meta:
        model = Profile
        fields = ['location', 'birth_date', 'year']

    location = forms.CharField(
        label = 'Укажите адрес',
        max_length=200,
        required = False,
        widget = forms.TextInput(attrs={'class': 'form-control'})
    )

    birth_date = forms.DateField(
        label = 'Укажите дату рождения',
        required = False,
        widget = NumberInput(attrs={'type': 'date'})
        #widget = forms.DateInput(attrs={'class': 'form-control'})
    )

    year = forms.ChoiceField(
        label='Год обучения',
        required = False,
        choices = SchoolYears.choices()
    )

class UserTypeForm(forms.Form):
    usertype = forms.ChoiceField(
        label = 'Тип профиля',
        required = True,
        choices = UserType.choices_safe()
    )#forms.CheckboxInput(required=False, label='')