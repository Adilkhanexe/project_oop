object Form2: TForm2
  Left = 0
  Top = 0
  Caption = 'Form2'
  ClientHeight = 425
  ClientWidth = 620
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  TextHeight = 15
  object MemoLog: TMemo
    Left = 192
    Top = 128
    Width = 185
    Height = 89
    Lines.Strings = (
      'MemoLog')
    TabOrder = 0
  end
  object BtnStart: TButton
    Left = 248
    Top = 240
    Width = 75
    Height = 25
    Caption = 'Start'
    TabOrder = 1
    OnClick = BtnStartClick
  end
  object IdTCPServer1: TIdTCPServer
    Bindings = <>
    DefaultPort = 0
    OnExecute = IdTCPServer1Execute
    Left = 584
    Top = 16
  end
end
