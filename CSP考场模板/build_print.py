from pathlib import Path
import re,html
from reportlab.pdfgen import canvas
from reportlab.platypus import SimpleDocTemplate,Paragraph,Spacer,Preformatted,KeepTogether,PageBreak
from reportlab.lib.styles import ParagraphStyle
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.lib import colors
from reportlab.lib.enums import TA_CENTER
from pypdf import PdfReader
p=Path('CSP考场模板'); out=Path('output/pdf');out.mkdir(parents=True,exist_ok=True)
tmp=Path('tmp/pdfs');tmp.mkdir(parents=True,exist_ok=True)
pdfmetrics.registerFont(TTFont('Song','C:/Windows/Fonts/simsun.ttc',subfontIndex=0))
styles={
 'body':ParagraphStyle('b',fontName='Song',fontSize=10,leading=15,spaceAfter=5,wordWrap='CJK'),
 'h1':ParagraphStyle('h1',fontName='Song',fontSize=20,leading=28,spaceAfter=12),
 'h2':ParagraphStyle('h2',fontName='Song',fontSize=13,leading=19,spaceBefore=10,spaceAfter=7,keepWithNext=True),
 'code':ParagraphStyle('c',fontName='Song',fontSize=9.5,leading=12.5,leftIndent=6,spaceAfter=8),
}
story=[]
def para(s,style='body'):
 s=s.replace('**','').replace('`','')
 return Paragraph(html.escape(s),styles[style])
def code(s):
 lines=[]
 for line in s.splitlines():
  line=line.expandtabs(4)
  # Printable wrapping, continuation indented. Comments and source remain readable.
  while pdfmetrics.stringWidth(line,'Song',9.5)>492:
   cut=len(line)
   while pdfmetrics.stringWidth(line[:cut],'Song',9.5)>480:cut-=1
   at=line.rfind(' ',0,cut)
   if at>cut//2:cut=at
   lines.append(line[:cut]);line='    '+line[cut:].lstrip()
  lines.append(line)
 return Preformatted('\n'.join(lines),styles['code'])
md=(p/'CSP算法速查手册.md').read_text(encoding='utf-8')
# Omit duplicate plain directory, replace local-file-dependent usage.
md=re.sub(r'## 目录\n.*?(?=## 01)', '',md,flags=re.S)
md=md.replace('也可以在本地使用 `#include "templates.hpp"`，提交时把需要的定义展开到源文件中。不要只提交依赖本地头文件的 main。','纸质资料中的所有模板均已展开；最后附独立的 Dijkstra 和线段树完整程序。')
md=re.sub(r'## 附录 B：线段树.*?(?=## 你原代码)', '',md,flags=re.S)
chunks=re.split(r'(?=^## )',md,flags=re.M)
for chunk in chunks:
 block=[];in_code=False;buf=[]
 for line in chunk.splitlines():
  if line.startswith('```'):
   if in_code:block.append(code('\n'.join(buf)));buf=[]
   in_code=not in_code;continue
  if in_code:buf.append(line);continue
  if not line.strip():continue
  if line.startswith('# '):block.append(para('CSP 算法模板 · 日常学习风格版','h1'));continue
  if line.startswith('## '):block.append(para(line[3:],'h2'));continue
  if line.startswith('|'):
   if '---' in line or '题目特征' in line:continue
   parts=[x.strip() for x in line.split('|')[1:-1]]
   block.append(para(' → '.join(parts)));continue
  block.append(para(line))
 # Prevent most algorithm sections from splitting across pages.
 if re.match(r'## \d\d ',chunk):story.append(KeepTogether(block))
 else:
  if chunk.startswith('## 附录 A') or chunk.startswith('## 原代码提醒'):
   story.append(PageBreak())
  story.extend(block)
for title,name in [('附录 B：Dijkstra 完整程序','Dijkstra完整示例.cpp'),('附录 C：线段树完整程序','线段树_区间加区间和.cpp')]:
 story.append(PageBreak());story.append(para(title,'h2'))
 s=(p/name).read_text(encoding='utf-8')
 if name.startswith('Dijkstra'):
  story.append(para('输入：n m s，接着 m 行有向边 u v w（w≥0）。输出源点到各点距离，不可达输出 -1；按实际题意修改。无向图需加反向边。'))
 else:story.append(para('输入 n m、n 个数；1 l r k：区间加 k；2 l r：区间求和。1 下标，n≤100000。多组数据须清空 lazy。'))
 # Split complete programs at a function boundary, never leave a few trailing lines.
 boundary = 'int main()' if name.startswith('Dijkstra') else '// 区间查询'
 split = s.index(boundary)
 story.append(code(s[:split]))
 story.append(PageBreak())
 story.append(para(title + '（续）','h2'))
 story.append(code(s[split:]))
class NumberedCanvas(canvas.Canvas):
 def __init__(self,*a,**kw):super().__init__(*a,**kw);self.saved=[]
 def showPage(self):self.saved.append(dict(self.__dict__));self._startPage()
 def save(self):
  total=len(self.saved)
  for state in self.saved:
   self.__dict__.update(state);self.setFont('Song',9.5)
   self.drawString(42,817,'CSP 算法模板 | C++17 | 日常学习风格版')
   self.drawRightString(553,25,f'{self._pageNumber} / {total}')
   self.setStrokeColor(colors.HexColor('#999999'));self.line(42,808,553,808)
   super().showPage()
  super().save()
file=out/'CSP算法模板_日常学习风格版.pdf'
SimpleDocTemplate(str(file),pagesize=(595.276,841.89),leftMargin=42,rightMargin=42,topMargin=42,bottomMargin=42,title='CSP 算法模板 · 日常学习风格版',author='').build(story,canvasmaker=NumberedCanvas)
r=PdfReader(file);print('PDF pages:',len(r.pages))
for i,page in enumerate(r.pages):
 text=page.extract_text()
 assert len(text)>60,(i,len(text))
print(file.resolve())
import pypdfium2 as pdfium
from PIL import Image,ImageOps,ImageDraw
doc=pdfium.PdfDocument(str(file));thumbs=[]
for i in range(len(doc)):
 im=doc[i].render(scale=1).to_pil().convert('RGB');im.save(tmp/f'page-{i+1:02d}.png')
 im.thumbnail((298,421));im=ImageOps.expand(im,border=5,fill='gray');thumbs.append(im)
w=4*308;hh=((len(thumbs)+3)//4)*431
sheet=Image.new('RGB',(w,hh),'white')
for i,im in enumerate(thumbs):sheet.paste(im,((i%4)*308,(i//4)*431))
sheet.save(tmp/'contact.png')
